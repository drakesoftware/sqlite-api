#!/usr/bin/perl 


package main;


our %membervars;
our %memberarrays; #Handled separately


my %typemap=
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

sub isValidType
{
    my $key=shift;
    return exists($typemap{$key});
}

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


#*****************************
#Return all variables for current class
sub getVars
{
    return sort(keys(%membervars));
}
#***********************
sub getVarCount
{
    my $count=keys(%membervars);
    return $count;
}
#***************************

sub clearVars
{
    %membervars=();
}
#************************************************
#Return type, translate 

sub getTypeByName
{
    my $name=shift;
    return $membervars{$name};
}

sub getNativeType
{
    my $type=shift;
    return $typemap{$type};
}


sub getNativeTypeByName
{
    my $name=shift;
    return $typemap{getTypeByName($name)};
}


sub getArrayTypeByName
{
    my $name=shift;
    return getNativeType($memberarrays{$name});
}


#***************************
sub setType
{
    my $name=shift;
    my $type=shift;
    my $arraytype=shift;
    $membervars{$name}=$type;
    if (defined($arraytype))
    {
        $memberarrays{${name}}=$arraytype;
    }
}

#######################################################
#Python3 module generator functions
sub printPythonMembers
{
    my $fh=shift;
    my $classname=shift;
    my $cmdid=shift;
    print $fh "    def __init__(self):\n";
    my $membercount=0;
    print $fh "        self.Data={}\n";
    print $fh "        self.Data[\'CommandID\']=$cmdid\n";
    print $fh "\n\n"; 
}

sub printPythonMemberFunctions
{
    my $fh=shift;
    my $classname=shift;
    for my $name (getVars())
    {
        my $typename=getTypeByName($name);
        print $fh "    def set${name}(self,val):\n";
        print $fh "        self.Data[\'${name}\']=val\n\n"; 
    
        print $fh "    def get${name}(self):\n";
        print $fh "        return self.Data[\'${name}\']\n\n"; 

        print $fh "    def is${name}Valid(self):\n";
        print $fh "        return \"${name}\" in self.Data\n\n";
    
    }

       print $fh "    def setMessage(self,msgstr):\n";
       print $fh "        self.Data=json.loads(msgstr)\n\n";
  
       print $fh "    def getMessage(self):\n";
       print $fh "        return json.dumps(self.Data)\n\n";
  
       print $fh "    def setObject(self,obj):\n";
       print $fh "        self.Data=obj\n\n";
  
       print $fh "    def getObject(self):\n";
       print $fh "        return self.Data\n\n";
 
}


sub createPythonModule
{
    my $classname=shift;
    my $cmdid=shift;

    my $ModuleFile="$classname.py";

    if (!open(MODULEFILE,">$ModuleFile"))
    {
        print "Unable to open Class file header: $ModuleFile\n";
        exit(1);
    }
 
    print MODULEFILE "#!/usr/bin/python3\n";
    print MODULEFILE "import json\n\n\n";
#print MODULEFILE "import TServerMessage\n\n\n";

    print MODULEFILE "class ${classname}:\n";
    
    printPythonMembers(*MODULEFILE,$classname,$cmdid);
    printPythonMemberFunctions(*MODULEFILE,$classname,$cmdid);
   
    close MODULEFILE;
    chmod ($ModuleFile,0755);

}




#*********************************************************************
sub printIncludes
{
    my $fh=shift;
    my $classname=shift;

#$classname=~s/\./\_/g;
    print $fh "#ifndef __${classname}_h\n";
    print $fh "#define __${classname}_h\n\n";
    print $fh "#include <stdio.h>\n";
    print $fh "#include <stdlib.h>\n";
    print $fh "#include <string>\n";
    print $fh "#include \"TServerMessage.h\"\n";
    print $fh "\n\n\n";
}


1;

