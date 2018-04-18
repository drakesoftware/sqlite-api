#!/usr/bin/perl 

use Getopt::Long;
use strict qw(vars);
#use diagnostics;

use FindBin qw($Bin);
use lib "$Bin";

use pygen;

require msgparse;
my $PythonOutput=0;
my $infile;
my $linenumber;
my $MessageFormatVersion=0;


##toComplete
#sub getStringConverter
#{
#    my $type=shift;
#    my $varname=shift;
#    if( $type eq "int"  ||
#        $type eq "unsigned int"  ||
#        $type eq "unsigned long long int"  ||
#        $type eq "short unsigned int"  ||
#        $type eq "float"  ||
#        $type eq "double")
#    {
#       return "std::to_string($varname)";
#    }
#    elsif ($type eq "bool")
#    {
#        return "${name}?"true";"false";
#    }
#}


our %typemap=
(
    "String" => "std::string",
    "Int" => "int",
    "UInt" => "unsigned int",
    "UInt64" => "unsigned long long int",
    "UShort" => "short unsigned int",
    "Short" => "short int",
    "Float" => "float",
    "Double" => "double",
    "Bool" => "bool",
    "Array" => "std::vector"
);

sub printClassMembers
{
    my $fh=shift;
    print $fh "    const int mFormatVersion = $MessageFormatVersion;\n";
#    for my $name (getVars())
#    {
#        my $typename=getNativeTypeByName($name);
#        if ("$typename"  eq "std::vector")
#        {
#            print $fh "    ${typename}<".getArrayTypeByName($name)."> m${name};\n";
#        }
#        else
#        {
#            print $fh "    ${typename} m${name};\n";
#        }
#    }
    print $fh "    char* json_string;\n \n";
}
#*******************************************************************
sub printMemberFunctions
{

    my $fh=shift;

    ############################################
    #Print set/get functions for message members
    for my $name (getVars())
    {
        my $nativetype;
        my $typename=getTypeByName($name);
        #set<name>
        if ($typename eq "Array")
        {
            $nativetype=getNativeTypeByName($name) . "<" . getArrayTypeByName($name) . ">" ;
            print $fh "    void set${name}($nativetype& value)\n"; #function name and args
        }
        else
        {
            $nativetype=getNativeTypeByName($name);
            print $fh "    void set${name}($nativetype value)\n"; #function name and args
        }

        print $fh "    {\n";

        print $fh "        write(\"${name}\",value);\n";
        print $fh "    }\n\n";
      

        ###########################################
        #get<name>
        if (getTypeByName($name) eq "Array")
        {
            print $fh "    $nativetype get${name}(std::vector<",getArrayTypeByName($name),"> arr)\n\n";   #function name
            print $fh "    {\n";
            print $fh "        return getArray(\"${name}\",arr);\n";
        }
        else
        {
            print $fh "    $nativetype get${name}(void)\n\n";   #function name
            print $fh "    {\n";
            print $fh "        return get",getTypeByName($name),"(\"${name}\");\n";
        }


        print $fh "    }\n\n";


        ###########################################
        #is<name>Valid
        print $fh "    bool is${name}Valid(void)\n";
        print $fh "    {\n";
        print $fh "        return isValid(\"${name}\");\n\n";
        print $fh "    }\n\n";
    }

}
#*******************************************************************
sub printMessageFunctions
{
    
    my $fh=shift;
    #########################################
    #Set the JSON object with an incoming JSON string 
    print $fh "    void setMessage(std::string jsonstring)\n";
    print $fh "    {\n\n";

    print $fh "        TServerMessage::setMessage(jsonstring);\n";

    print $fh "\n"; 
    print $fh "    }\n\n";


    #Set the JSON object, so it's values can be retrieved
    print $fh "    void set(Json::Value obj)\n";
    print $fh "    {\n\n";

    print $fh "        TServerMessage::set(obj);\n";

    print $fh "\n"; 
    print $fh "    }\n\n";



    #
    #write getMessage()
    print $fh "    std::string getMessage(void)\n";
    print $fh "    {\n";
    print $fh "        return TServerMessage::getMessage();\n";
    print $fh "    }\n\n";
    
#  Write get() 

    print $fh "    Json::Value get(void)\n";
    print $fh "    {\n";
    print $fh "        return TServerMessage::get();\n";
 
    print $fh "    }\n\n";


}
#*********************************************************************
sub printClass
{
    my $fh=shift;
    my $classname=shift;
    my $cmdid=shift;

    print $fh "class C${classname}:public TServerMessage\n"; 
    print $fh "{\n";
   
    printClassMembers($fh); 
   
    print $fh "public:\n";
    print $fh "    static const unsigned int mCmdID=",sprintf("0x%x",${cmdid}),";\n";

    print $fh "    C${classname}(void)\n";
    print $fh "    {\n";
    print $fh "        setCmdID(mCmdID);\n";
    print $fh "    }\n";

    print $fh "    C${classname}(Json::Value obj):TServerMessage(obj)\n";
    print $fh "    {\n";
    print $fh "        set(obj);\n";
    print $fh "        setCmdID(mCmdID);\n";
    print $fh "    }\n";





    printMemberFunctions($fh);
    printMessageFunctions($fh);
}
#*********************************************************************
sub endClass
{
    my $fh=shift;
    my $classname=shift;

    print $fh "};\n\n\n";
    print $fh "\n\n#endif";
}

#*********************************************************************
sub printCPP
{
    my $fh=shift;
    my $classname=shift;
    
    print $fh "\n\n\n";
    print $fh "#include ${classname}.h\n\n\n"
}
#*********************************************************************
sub createClassFile
{
    my $classname=shift;
    my $cmdid=shift;

    my $classfileheader="$classname.h";
    my $classfile="$classname.cpp";

    if (!open(CLASSHEADER,">$classfileheader"))
    {
        print "Unable to open Class file header: $classfileheader\n";
        exit(1);
    }
    if (!open(CLASSFILE,">$classfile"))
    {
        print "Unable to open Class file header: $classfileheader\n";
        exit(1);
    }
    
    #Create a blank .cpp file
	print CLASSFILE "#include \"${classfileheader}\"\n";

    printIncludes(*CLASSHEADER,$classname);
    printClass(*CLASSHEADER,$classname,$cmdid);
    endClass(*CLASSHEADER,$classname);
   
    close CLASSHEADER;
    close CLASSFILE;


}
#*********************************************************************8
sub getMembers
{
    while(<$infile>)
    {
        $linenumber++; 
        if (/^#/)
        { 
            next;
        }
        chomp();
        if (/^end/)
        {
            last;
        }
        if (/(.*?)#(.*)/)
        {
            $_=$1;
        }
        if (length($_))
        {
            if (/\s+(\S+)\s+(\S+)\s+(\S+)/)
            {
                if ($2 ne "Array")
                {
                    printf("Invalid format at line %d\n",$linenumber);
                    printf("%s\n",$_);
                }
                
                #$1=name $2=type=array,$3=array element type
                if (!isValidType($3))
                {
                    printf("Line: %d Invalid field type \"%s\"",$linenumber,$3);    
                    printf("%s\n",$_);
                    next;    
                }
                setType($1,$2,$3); 
            }
            elsif (/\s+(\S+)\s+(\S+)/)
            {
               #$1=name $2=type
                if (!isValidType($2))
                {
                    printf("Line: %d Invalid array element type \"%s\"",$linenumber,$2);    
                    printf("%s\n",$_);
                    next;    
                }
                setType($1,$2); 
            } 
                    
        }
    }
}

#*********************************************************************8
sub findEntry
{
    while(<$infile>)
    {
        $linenumber++;
        if (/^#/)
        { 
            next;
        }
        if (/^Message\s+(\S+)\s+(\S+)/)
        {
            return ($1, hex($2)); #Don't need 3rd arg (TS/UI) 
        }
        if (/^(Version)\s+(\S+)/)
        {
            return ($1,hex($2));
        } 

    }
    return (undef,undef);
}
#*********************************************************************8
sub printClassHeader
{
print CLASSLISTHEADER  "#ifndef __classes_h\n";
print CLASSLISTHEADER "#define __classes_h\n";
print CLASSLISTHEADER "\n\n";
}


#my $result=GetOptions('-directory=s'=>\$TargetDir,'r=s'=>\$rev,'la'=>\$List,'norecurse'=>\$NoRecurse,'ld'=>\$ListDiff,'ls'=>\$ListSame,"i"=>\$Ignore);


if ($#ARGV < 0)
    {
    exit(1);
    }

GetOptions('python' => \$PythonOutput);

my $fname=$ARGV[0];



my $classname;
my $cmdid;
my $PythonMainModule="messages.py";
my $PythonModuleList="pythonmodulefiles.mk";

if (!open($infile,"$fname"))
{

    printf("Unable to open input file $fname\n");
    exit(1);
}

if ($PythonOutput)
{
    if (!open(PYTHONMAINMODULE,">>${PythonMainModule}"))
    {
        printf("Unable to open ${PythonMainModule}\n");
        exit(1);
    }

    if (!open(PYTHONMODULELIST,">>${PythonModuleList}"))
    {
        printf("Unable to open ${PythonModuleList}\n");
        exit(1);
    }
    $linenumber=0;
    while(1)
    {
        my ($name,$value)=findEntry(); #value is message version or CommandID if a message entry
        if (!defined($name))
        {
            last;
        }
        if ($name eq "Version")
        {
            $MessageFormatVersion=$value;
            next;
        }
        else #Message entry
        {
            #printf("%s %d\n",$name, $value);
            getMembers();
            createPythonModule($name,$value,getVars());
            print PYTHONMAINMODULE "from $name import ${name}\n";
            print PYTHONMODULELIST "${name}.py\n";
            print "${name}.py\n";
        }
        clearVars();
    }   
    close PYTHONMAINMODULE;
    close PYTHONMODULELIST;

    #Make sure they're executable
    chmod($PythonMainModule,0755);
    chmod($PythonModuleList,0755);
}
else
{
    if (!open(CLASSLISTHEADER,">>classes.h"))
    {
        printf("Unable to open classes.h\n");
        exit(1);
    }

    if (!open(CLASSCPPLIST,">>classcppfiles.mk"))
    {
        printf("Unable to open classcppfiles.mk\n");
        exit(1);
    }

    if (!open(CLASSHEADERLIST,">>classheaderfiles.mk"))
    {
        printf("Unable to open classesheaderfiles.mk\n");
        exit(1);
    }
    $linenumber=0;
    #printClassHeader(*CLASSLISTHEADER);
    while(1)
    {
        my ($name,$value)=findEntry();
        if (!defined($name))
        {
            last;
        }
        if ($name eq "Version")
        {
            $MessageFormatVersion=$value;
            next;
        }
        else #Message entry
        {
            #name=classname, value=cmdid
            printf("%s %d\n",$name, $value);
            getMembers();
            createClassFile($name,$value);
            print CLASSLISTHEADER "#include \"${name}.h\"\n";
            print CLASSHEADERLIST "${name}.h\n";
            print CLASSCPPLIST "${name}.cpp\n";
        }
        clearVars();
    }


#print CLASSLISTHEADER "\n\n#endif\n\n";

    close CLASSLISTHEADER;
    close CLASSCPPLIST;
    close CLASSHEADERLIST;
} 
