Database simulation using C
the data base sypoorts the following commands

        SELECT * FROM TableName WHERE Field2="moh"
        DELETE FROM TableName WHERE Field1="value"
        UPDATE TableName SET Field1="newvalue" WHERE Field2="moh"
        SELECT * FROM TableName JOIN TableName2 ON TableName.Field1 = TableName.Field2
        INSERT INTO TableName (Field1="hoa", Field2="man")
        CREATE TABLE TableName FIELDS [Field1, Field2]
        DROP TABLE TableName

---------------------------------------
#Data base Files#

    each table is savedin 2 files
        1-for the fomat of the table (TableName_content.txt)
                N
                col_name1
                col_name2
                col_name3
                col_name4
                .
                .
                .
            where N is the number of cols
        2- content file (TableName_content.txt)
            every row has N words seperated by spaces
            
--------------
DataBase does not support any Tables names or table values or column valus that contain spaces.

        