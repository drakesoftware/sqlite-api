 

SQLite-API
Introduction
The library helps serialize or deserialize a C++ class using SQLite in the backend. Its usage is simple and yet it helps the user avoid all details related to the database operations. Serializing or deserializing data is an internal feature of many applications and handling the same using SQLite involves writing DDL and DML related code for SQL. This framework does all that behind the scenes, without the user to worry much about the implementation details.

Background
The code has been written for diverse use cases. It assumes that the user won't have to write any code related to SQLite. Still, the user shall have the flexibility of deciding as to which properties/fields to serialize and how?

Using the code
There are two ways we can use the API. One is by using a class from the API called Catalogue that can help us manipulate data in the database directly. Following is the sample code;

    
          Catalogue freqTbl("bit.db", "freqtbl", schmTbl);
          freq.Set("Freq", 1090.8);
          freq.Set("Type", 0);
          freq.Save();
      
          freq.Set("Freq", 978.5);
          freq.Set("Type", 1);
          freq.Save();
      
Here bit.db is the name of the database, freqtbl is the name of table and schmTbl is the schema for the table. The schema, schmTbl, can be defined as under;

schema = 
          {
              {"Freq", PLAT_DBL},
              {"Type", PLAT_DBL}
          };
      
Pre-caching the schema of tables
When using the Catalogue class, we may not want to provide schema every time we need to do something with the data. So there is a helper class called DBCache that can be used to register all tables and schemas to be used in the app. And later, when required, the user can simply grab an instance of Catalogue class from the singleton of DBCache class, for the table that requires any data manipulation. Following is how tables and schemas can be registered with DBCache

            DBCache::Select("c1", "test.db");

            DBCache::Current()
            .AddTable("bit", // Re-create table (if schema changed)
            {   {"Type", PLAT_BOOL},
                {"Result", PLAT_BOOL}, 
                {"Val", PLAT_DBL},
                {"I", PLAT_DBL},
                {"Q", PLAT_DBL},
                {"Time", PLAT_INT},
                {"Date", PLAT_INT},
                {"Run", PLAT_INT}
            });
            .AddTable("limtbl",
            {   {"MinLim", PLAT_DBL},
                {"MaxLim", PLAT_DBL},
                {"Type", PLAT_INT}
            });
      
Here, c1 is the name of the cache. The Select function makes the cache named 'c1' the current cache that can be simply retrieved using the singleton, DBCache::Current(). The AddTable functions take table name and schema as the only two arguments. Following is how a table can be retrieved from DBCache to perform an INSERT operation.

            // Insert
            up_catalogue bit;
            if (DBCache::Current().Get("bit", bit)) // If tableName exists then
            {
                bit->Set("Type", 0); // Set data
                bit->Set("Result", true);
                bit->Set("Val", 1000);
                bit->Set("I", 0);
                bit->Set("Q", 0);
                bit->Set("Time", static_cast(time_t(NULL)));
                bit->Set("Run", 1);
                bit->Save(); // Save data to database
    
                bit->Set("Type", 1);
                bit->Set("Result", false);
                bit->Set("Val", 300);
                bit->Set("I", 0);
                bit->Set("Q", 0);
                bit->Set("Time", static_cast(time_t(NULL)));
                bit->Set("Run", 2);
                bit->Save();
            }
      
There are a couple of interesting features of DBCache. It saves the schema of the table as a hash in the database in a separate table called sys_cache. And then every time DBCache is initialized, it looks for any changes in the schema and if there are any, it acts as per the chosen policy; either raise an exception or delete the old table and create a new one. DBCache also helps control resources in a way that we can have a separate cache for individual modules. This way we only register the tables that are required for the respective modules. 

Creating Matsers in database
There is a provision to add masters to the database when the application starts. Such masters, once added, are only checked for schema integrity everytime the app starts. Adding a master is simply a quick and easy way to add a table and data together in one step. A master is expected to be a read-only entity, although there isn't a provision as of now to really keep it read-only. Following is how a master can be added using DBCache

        DBCache::Select("c1", "test.db");
        Master mstFreqTbl("freqtbl", // Re-create master table (if existing table schema changed) in database
        {
            {"Freq", PLAT_DBL},
            {"Mod", PLAT_INT},
            {"Type", PLAT_INT},
            {"Datetime", PLAT_DT} //Added new datetime type
        });

        mstFreqTbl
        // #0 Tx & Rx Synthesizer in MHz, Rx IQ Demodulator, Tx IQ Modulator, Tx IQ Gain Bal
        .Add(0.200, 10, 0, Datetime::Current()) //Saving current datetime
        .Add(29.9999, 10, 0, Datetime::Current())
        .Add(30, 10, 0, Datetime::Current())
        .Add(500, 10, 0, Datetime::Current())
        .Add(1100, 10, 0, Datetime::Current())
        .Add(2200, 10, 0, Datetime::Current())
        // #1 Tx Reference Tuning Freq in GHz, Cal Detector
        .Add(.5, 10, 1, Datetime::Current())
        .Add(1.100, 10, 1, Datetime::Current())
        .Add(2.200, 10, 1, Datetime::Current());

        DBCache::Current().AddMaster(mstFreqTbl); // Add master table with data
      
Converting a class into a persistable entity
The second way to perform any database manipulations is to derive from the PersistableBase class from the API and use its protected/public functions. Here is how the same can be accomplished:

To serialize/deserialize any class, there are four simple steps that we need to follow. First is to derive the class in question from the PersistableBase class. Second is to override the functiononSetData(). The third is to override the function onGetData(Columns& cols). And finally, override the getSchema() function. Deriving from the PersistableBase class renders the class in quetion with the primary functions to Create/Read/Update/Delete data as required. Let's assume there is a class called AppData that we need to serialize.

class AppData
      {
      
      private:
          int m_userId = 0;
          string m_key;
          string m_value;
      };
      
We'll need to derive it from PersistableBase class;

class AppData: public PersistableBase
      {...
      
And then we'll need to override the three functions to support db operations;

    /**
           * The three functions that need to be created 
           * in the class that has to be serialized. 
           * All three functions are called internally 
           * and should not be required to be called 
           * from outside at any point of time. 
          */
          schema getschema() const override;  //to define schema
          void onSetData() override;            //to get data from object into db
          void onGetData(Columns& cols) override;   //to get data from db into object
          /*----------------------------------------------*/
      
These three functions (getSchema, onSetData, onGetData) are really just a declaration that the author of any data class, like AppData, needs to make as to what data from the class needs to be pushed to the DB and how the same data shall be used to populate any new object of that type. onSetData() is called internally from PersistableBase class when Save() is called on any data class deriving from PersistableBase. Inside the onSetData() functions, it is assumed that the author of the data class has used the protected set functions to push data from the class to a data collector class (Columns) which would then process the data as required. And the onGetData(Columns) function is also called internally from PersistableBase at the time of creation of new objects when user fetches data from DB. This function also assumes that the author of the deriving data class has used the Columns object passed in its argument to populate the data class. These two functions use the getSchema function to maintain what would be the schema of the table where data is saved or retrieved from. The complete code for the appdata.h file hence becomes;

#include "persistablebase.h"
      using namespace std;
      
      class AppData: public PersistableBase
      {
      
      private:
          int m_userId = 0;
          string m_key;
          string m_value;
      
      public:
          //A constructor that defines what database and the table in it, the data will go into 
          AppData(const char* dbName, const char* tableName);
      
          //Another that just pre defines what table or database the object will always save to
          AppData::AppData(const int userId, const string &key, const string &value):
          PersistableBase{"tel.db", "data1"}
          {
                m_userId = userId;
                m_key = key;
                m_value = value;
          }  
      
          /**
           * The three functions that need to be created 
           * in the class that has to be serialized. 
           * All three functions are called internally 
           * and should not be required to be called 
           * from outside at any point of time. 
          */
          schema getschema() const override;  //to define schema
          void onSetData() override;            //to get data from object into db
          void onGetData(Columns& cols) override;   //to get data from db into object
          /*----------------------------------------------*/
      };
      
After we have setup the pre-requisites, we can create the object as;

      AppData anySetting("app.db", "data1");
      
Or just; AppData anySettings;

Save in DB
We can save the object as;

Using Catalogue from DBCache
    //Insert
      up_catalogue freq;
          if (DBCache::Current().Get("freqtbl", freq))
          {
              freq->Set("Freq", 1090.8);
              freq->Set("Type", 0);
              freq->Save();
      
              freq->Set("Freq", 978.5);
              freq->Set("Type", 1);
              freq->Save();
          }
      
Using PersistableBase
    //Insert
          AppData distance(1, "distance", "25Ft");
          distance.Save();
      
          AppData port(1, "port", "A");
          port.Save();
      
Read from DB
We can retrieve the objects from database as;

Using Catalogue from DBCache
    //Select
      up_catalogue limit;
          if (DBCache::Current().Get("limtbl", limit))
          {
              vector veclimit = limit->Select("Type = 0");
              for(Catalogue& limiCat: veclimit)
              {
                  cout << limiCat.getDouble("MaxLim") << endl;
                  //Or
                  double minLim;
                  if(limiCat.get(minLim, "MinLim"))
                  {
                      cout << minLim << endl;
                  }
              }
          }
      
Using PersistableBase
    //Select
          auto all = PersistableBase::Select(AppData());
          for(auto ad: all){
              cout << ad.key() << ":" << ad.value() << endl;
          }
      
Read selectively
We can also apply basic filter to the list of items we want to fetch from DB. To filter data selection, simply pass the SQL string that is required to create the WHERE clause (but, without the WHERE keyword).

            //SELECT (filtered) (Catalogue class has wrapper function)
                  vector filtered = PersistableBase::Select(AppData(), "key = 'distance' AND value = '25Ft'");
            
The other way is to use the inbuilt API for filters. Filter object can be passed as second argument to the PersistableBase::Select(...) function. We need to call apply_filter(const char* key, t value) from filter.h. For example, to apply filter to the list so as to get only rows with key equal to "distance", we need to pass apply_filter("key", string("distance")) as second argument of the PersistableBase::Select(...) function. To apply multiple filters separated by logical operators, we can simply go on adding more apply_filter(...) after dot and the logical operator required in the clause. An example can clarify better the syntax.

      //Select (filtered)(Catalogue class has wrapper function)
            auto selected = PersistableBase::Select(AppData("app.db", "data1"),
                  apply_filter("key", string("distance")    // first filter
                  .AND()                                    // logical operator
                  .apply_filter("value", string("25Ft")));  // second filter and so on...
            
            for(auto ad: selected){
                  cout << ad.key() << ":" << ad.value() << endl;
            }
      
      
Apart from AND() the only other logical operators available as of now are OR() and NOT(). The apply_filter(...) function has third (optional) argument that is an enum (FilterOperatorEnum). It is the operator acting between the two arguments of apply_filter(...). By defualt, it is EQ(==). The other options are; LT(<), LTQ(<=), GT(>), GTQ(>=) and NQ(!)

Update existing records in DB
We can update any object right back in the database as;

    //Update(Catalogue class has wrapper function)
          AppData& ad = all.at(5);
          ad.setKey("stars");
          ad.Update();
      
Delete records in DB
And we can delete any object in the database as;

    //Delete(Catalogue class has wrapper function)
          all.at(10).Remove();
      
The library has an PersistableBase class that offers public interface for the basic DB operations (Save/Update/Delete/List). This class derives from the Column class. The Column class is quite interesting in more than one ways. Firstly, this is the class that actually holds data for the class either when it is moving from the class to do or when it moves from DB to class. Hence, this class has to be capable of holding multiple types of data. It uses four separate templated data collections to hold this data. And then there is Table class that is a kind of controller for the DB class, which is a wrapper around the actual sqlite3 API. The function of the Table class is to provide standard interface that is identifiable with database opeartions. This class, creates SQL strings using a helper library (SqlScriptHelper) and passes the same to the DB class. All databse related functions are written in DB class. All SQL quries creating code is written in SqlScriptHelper class. The public interface to basic db operations are all provided in Table class. And to access those functions easily, PersistableBase class provides some virtual functions, that are called internally when Save, Update or List are run. And PersistableBase is the class we can derive from to make a class serializable/deserializable.

Points of Interest
There is an interesting feature that the current code leads to. Although the code right now handles only the simple data types, but with a small enhancement in code, we can easily make it handle composite types or user-defined types. This would mean that any object can be saved in SQLite DB by just deriving from PersistableBase even if the properties/fields we are trying to serialize are complex user-defined types.

      //UserData has both, a simple type (int m_userId), and a composite type (Appdata appData)
            class UserData: public PersistableBase //Derives from PersistableBase
            {
      
            private:
                int m_userId = 0;
                AppData appData //Already derives from PersistableBase
            };
            
The only pre requisite is that the composite fields should also derive from PersistableBase.

This does work as required but a preferred way of using the library would be via deriving from PersistableBase class. That would bring the benefits of encapsulation and create a standard implementation across the client app.


History
          Oct 30, 2018      : Provision to cache table schemas and add Masters      
          May 17, 2018      : Modified many variable and function names, created provision to filter data by passing string 
          April 30, 2018    : Added provision to read selectively from DB by applying filters to Select function.
          April 26, 2018    : Initial version.
      
