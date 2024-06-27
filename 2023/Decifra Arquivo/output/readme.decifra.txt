                    Delphi 7
                 Release Notes


=======================================================

This file contains important supplementary and late-
breaking information that may not appear in the main
product documentation. We recommend that you read this
file in its entirety.

For information on new features in this release, choose
"What's New" in the online Help contents.


=======================================================
CONTENTS

* OTHER RELEASE NOTES INCLUDED WITH THIS PRODUCT
* NOTES AND ISSUES:
* ABOUT EDITING THE REGISTRY
* PRODUCT INFORMATION ON THE WEB


=======================================================
OTHER RELEASE NOTES INCLUDED WITH THIS PRODUCT

* INSTALL contains system requirements and product
  installation information.

* DEPLOY contains information about redistributing your
  applications.

* LICENSE contains information on licensing allowances
  and limitations for this product and other Borland
  software that is bundled with it.

    The three files listed above, along with this file,
    are installed in your main product directory
    (default: C:\Program Files\Borland\Delphi7).

=======================================================
NOTES AND ISSUES

Deprecated Components

    Borland is deprecating the use of TSQLClientDataSet
    and TBDEClientDataSet. Borland recommends that you
    use TSimpleDataSet for simple, two-tier
    applications. TSQLClientDataSet and
    TBDEClientDataSet are no longer displayed on the
    Component palette. If you require backward
    compatibility, you can still access
    TSQLClientDataSet and TBDEClientDataSet in the
    Demos directory (by default, C:\Program
    Files\Borland\Delphi7\Demos).


    Borland is deprecating the use of the TServerSocket
    and TClientSocket from the unit ScktComp. It is
    recommended that you use the Indy components for
    socket operations. The TServerSocket and
    TClientSocket will no longer be installed on the
    component palette by default. If you require the
    use of these components then you can install
    the design time package named dclsockets70.bpl,
    found in your bin directory. For deployment with
    runtime packages, you will need to deploy rtl70.bpl
    and any other required packages.


Changes in StrUtils
-------------------

The StrUtils unit contains three sets of changes, all
relating to
multi-byte character set (MBCS) support.

   * Previously, LeftStr, RightStr, and MidStr each
     took and returned AnsiString values, and did not
     support MBCS strings. Each of these functions has
     been replaced by a pair of overloaded functions,
     one that takes and returns AnsiString, and one
     that takes and returns WideString. The new
     functions correctly handle MBCS strings. This
     change breaks code that uses these functions to
     store and retrieve byte values in AnsiStrings.
     Such code should use the new byte-level functions
     described below.

   * New functions LeftBStr, RightBStr, and MidBStr
     provide the byte-level manipulation previously
     provided by LeftStr, RightStr, and MidStr.

   * New functions AnsiLeftStr, AnsiRightStr, and
     AnsiMidStr are the same as the new AnsiStr
     LeftStr, RightStr, and MidStr functions, except
     that they are not overloaded with equivalent
     WideString functions.


XP Themes
---------

This build does not include a manifest for your
projects. To tell your applications to use version 6.0
of comctl32.dll you need to create and edit the
manifest yourself.

There are two ways to do this:

1. With a stand-alone manifest file added to the same
   directory of the executable.

2. With a manifest included in the executable as a
   resource.


For the stand-alone manifest follow these steps:

I) Using Notepad create a file called
   "Project1.exe.manifest" and add the following lines:

<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1"
manifestVersion="1.0">
<assemblyIdentity
processorArchitecture="*"
version="1.0.0.0"
type="win32"
name="Project1"/>
<description>Project1 Description</description>
<dependency>
<dependentAssembly>
<assemblyIdentity
type="win32"
name="Microsoft.Windows.Common-Controls"
version="6.0.0.0"
publicKeyToken="6595b64144ccf1df"
language="*"
processorArchitecture="*"   />
</dependentAssembly>
</dependency>
</assembly>

II) Save the file and place it in the same directory as
    the application executable.

If you don't like the idea of having the manifest as a
separated file you can include it into your executable
as a resource.

I) Create a resource file (again using Notepad)
   containing the following line:

   1 24 "Project1.exe.Manifest"

II) Compile the resource file with brcc32.exe with the
    following command:

   C:\Project1> brcc32 WindowsXP.RC

III) Now add the following line to your project's main
     form:

   {$R WindowsXP.RES}


NOTE: To automatically configure XP Themes in your
      project you can use the XP Application Wizard
      demo. For more information see the readme file in

       Demos\ToolsAPI\XPAppWiz


.NET compiler warnings
----------------------

The Delphi 7 dcc32 compiler now supports three
additional compiler warnings:  Unsafe_Type,
Unsafe_Code, and Unsafe_Cast.  These warnings are
disabled by default, but can be enabled with source
code compiler directives {$WARN UNSAFE_CODE ON},
compiler command line switches (dcc32 -W+UNSAFE_CODE),
and in Project|Options.

NOTE: There is no space before or after the "+" in the
      command line switch)

"Unsafe" in all three messages refers to types or
operations which static code analysis cannot prove to
not overwrite memory.  For example, data types that
carry bounds information at runtime such as Delphi
Strings or dynamic arrays can be bounds-checked at
runtime, and tracked in static analysis, to ensure that
memory accesses are within the limits of the allocated
data.  Data types that do not carry bounds information
at compile time or runtime (such as PChar) cannot be
proven safe.  Unsafe doesn't necessarily mean the code
is broken or flawed, it simply means that it uses
programming techniques that cannot be verified as safe
by static code analysis.  In a secured execution
environment such as .NET, such code is assumed to be
unsafe and a potential security risk.

Delphi 7 developers can use these new warnings to
identify data types, code or programming techniques
that may be difficult to port to a managed code
environment such as .NET.  These messages are not
specific to the .NET platform.  The warnings are
conservative - some things we warn about in D7 we might
actually be able to support in Delphi for .NET, but
we're not sure yet.  The warnings are not complete -
there may be problematic types or code situations for
which the Delphi 7 compiler does not issue an unsafe
warning.

Unsafe_Type:  Types such as PChar, untyped pointer,
untyped var and out parameters, file of <type>, 6 byte
reals (Real48), variant records (records containing
overlapping fields) and old-style objects

("TMyObject = object").

  * "old" object type

       c = object
         i: Integer;
         procedure p;
       end;

Unsafe_Code:  absolute variables, Addr(), Ptr(), Hi(),
Lo(), Swap() standard procedures, BlockRead and
BlockWrite, the Fail() standard procedure, GetMem(),
FreeMem(), and ReallocMem()

  * inline assembler blocks (asm end)
  * @ operator
  * modifying string index elements, e.g

      s := 'Hoho';
      s[2] := 'a'; // <-- "Unsafe code 'String index to
      var param'"

Unsafe_Cast:  Casting an object instance to a type that
is not an ancestor or descendent of the instance type,
casting a record type to anything else


Change to VCL SubComponents streaming
-------------------------------------

In Delphi 7, we corrected a problem where the csLoading
flag was never set for subcomponents, nor was the
subcomponent's Loaded method called. When a component that
has subcomponents is streamed, the subcomponents will have
their csLoading flag set and their Loaded method called.
This change creates a complication for any subcomponent
properties that are writable. If you allow your
subcomponent property to be assigned to an external
component reference then you cannot free your subcomponent
until it's owner's Loaded method is called otherwise the
streaming system will attempt to call the subcomponent's
Loaded method after the subcomponent has been freed.

Apache
------

Support for Apache 2 is for the 2.0.39 version. It will
work with later versions as long as they are binary
compatible.

NOTE: Due to a known bug with Apache 1.3.22, do not
      develop CGI programs or shared objects for this
      version.

      See http://bugs.apache.org/index.cgi/full/8538


UDDI Registry entries
---------------------

Some UDDI Registry entries contain an Accesspoint URL with
a bookmark tag at the end. When accessed these services
often generate the error message 'Method not allowed
(405)'.

Workaround:

Remove the tag from url in the global function
that returns the Interface:

     const
//        defURL = 'http://someservice/somewhere#tag';
//        is changed to

        defURL = 'http://someservice/somewhere';


Japanese Input system on Windows XP
-----------------------------------

The MS IME2002 Japanese input system included with
Windows XP can cause the Debugger in the IDE to
stall.

Workaround:

Remove Microsoft IME Character Code Dictionary from
your system dictionaries list. Simply un-checking this
option from the dictionary list will not solve the
problem. To remove this dictionary:

1) Open the IME Toolbar.
2) Select Tools|Properties and click on the Dictionary
   tab.
3) Select "Microsoft IME Character Code Dictionary"
   from the System dictionaries listbox
4) Click Remove.


Database
--------
* The provided DB2 driver is certified for DB2 version
  7 only. The client version and server version must
  match. BDE also supports DB2 version 7 only.

* The Oracle driver provided for dbExpress components
  is certified for Oracle 9i. BDE and ADO have been
  certified with Oracle 8.1.7 and 8.1.6. In all cases,
  the client version and server version must be the
  same.

* Oracle 9i types - ORACLE 9i types TIMESTAMP
  (introduced for JDK 1.3.1 compliance) and other types
  like XMLType, Temp tables, etc. are not yet
  supported. Use "DATE" instead.

* The Borland Database Engine CAB file (BDEINST.CAB) is
  no longer digitally signed. This CAB file is provided
  for backward compatibility. For new applications, we
  recommend deploying BDE with the BDE merge modules.

InterBase version support, Dialect 3 features
---------------------------------------------

Delphi 7 supports InterBase 6.5. The client version and
server version must be the same.

Currently, the driver does not support the ARRAY type.

To use the new InterBase 6.5 Dialect 3 features in BDE,
add an entry to your Windows registry under

 HKEY_LOCAL_MACHINE\SOFTWARE\Borland\Database Engine
 \Settings\Drivers\Intrbase\Db Open\SQLDIALECT

and set the String Value to "3".

When an InterBase alias is created, the new entry will
be available in the .CFG file.

To use InterBase 5.6, SQLDIALECT can be set to "1"
(existing IB aliases which do not have the SQLDIALECT
entry default to SQLDIALECT=1 or to the registry
setting when the SQLDIALECT entry is added to the
registry).


InterBase 6.5 implementation note
---------------------------------

If you are using a WebSnap application with InterBase
6.5, a local database connection is not recommended.
Instead, use a client/server connection specified by a
protocol-specific prefix to the path to the database.

Examples:

Local connection (not recommended):
    C:\PathTo\Database\DBName.gdb

TCP/IP connection on Windows:
    saturn:C:\PathTo\Database\DBName.gdb

TCP/IP connection on UNIX:
    jupiter:/usr/PathTo/Database/DBName.gdb

NetBEUI connection:
    \\venus\C:\PathTo\Database\DBName.gdb

IPX/SPX connection:
    mars@vol2:\PathTo\Database\DBName.gdb

If the WebSnap application is on the same machine as
the server, you can use localhost:

    localhost:C:\PathTo\Database\DBName.gdb


INTERBASE driver behavioral difference:
---------------------------------------

NUMERIC data type mapping.


   The dbExpress driver for INTERBASE  now maps NUMERIC
   data types to fldBCD instead of fldINT16, fldINT32,
   fldFLOAT.


dbExpress
---------

TSQLDataSet now has DefaultRowsetSize = 20 (Oracle
only). To use a different RowSetSize, add the
RowSetSize property manually (e.g., "RowsetSize = 200")
into dbxconnections.ini, for existing connections, or
into dbxdrivers.ini to have the RowSetSize property
included in new connections. RowsetSize can also be
modified in code, as shown here:

    "SQLConnection1.Params.Values['RowsetSize'] :=
    '200'"


dbExpress now includes MySQL 3.23.49 support with a new
driver (dbexpmysql.dll). To use the new driver, specify

    LibraryName = "dbexpmysql.dll"

in the Object Inspector or in dbxdrivers.ini.


dbExpress components and MySQL transactions
-------------------------------------------

MySQL doesn't support more than one active statement
per connection. To perform multiple SQL requests with a
single TSQLConnection the dbExpress components clone
connection. There is a bug in maintaining state
information on the no.of active statements with respect
to a TSQLConnection. This might cause serious failures
in transaction as the transaction might be started on
one connection and commit or rollback might be
happening on another connection.

Workaround:

Before you start a MySQL transaction make sure
TSQLConnection.ActiveStatements is 0. If it's not 0 then
close the TSQLConnection and reopen it and that should
clear it. All your transaction activity should then go
on a single connection.

DB2 notes for dbExpress
-----------------------

The provided DB2 driver is certified for DB2 version 7.
The client version and server version must be the
same. BDE also supports DB2 version 7.

Because of a known DB2 bug, the DB2 client returns only
one record at a time when a request for a block fetch
is made, even with RowsetSize option > 1.

Timestamp issues
----------------

The dbExpress DB2 driver rounds the TIMESTAMP
fractional value and there could be some precision lost
depending upon the data retrieved. When you apply back
changes the old value will not match the one on the
server and ApplyUpdates() will fail.

Workaround:

Make sure DB2 TIMESTAMP is not part of indexed columns
and set the DataSetProvider Update mode to WhereKey
instead of WhereAll.


Informix BLOB/CLOB fields (dbExpress)
-------------------------------------

A new global variable, InformixLob, is available to
allow you to work with Informix BLOB and CLOB fields.

The variable is required because Informix BLOB
(fldBLOB, fldstHBINARY) and CLOB (fldBLOB, fldstHMEMO)
fields are mapped as ftOraBlob and ftOraClob,
respectively, and the DataSnap resolver performs
special query generation when resolving Oracle
BLOB/CLOB fields, but not when resolving Informix
BLOB/CLOBs. To address this issue, set InformixLob
to True when using Informix BLOB/CLOBs, and switch it
back to False (the default) when using Oracle
BLOB/CLOBs.


NOTE: The new Informix driver (dbexpinf.dll) has been
      tested with Informix version 9.2.1 client/server
      and 9.2.1 client with SE server.

MSSQL driver - Known issues
---------------------------

1) Connection with a Blank Password/OS Authentication:

   With the current MSSQL driver you can't connect using a
   blank password or with OS Authentication = True.

   A patch will be made available soon at Borland's support
   download site.


2) Master detail:

   a) When trying to open a master-detail as a nested
      dataset and if the detail link field is an INTEGER
      type and defined NOT NULL you will get a SQL Error
      "SQL State:HY000, SQL Error Code: 0, Invalid Input
      parameter values. Check the status values for
      details"

      A patch will be made available soon at Borland's
      support download site.

   b) ApplyUpdates() on a master-detail with
      "poFetchDetailsOnDemand" = True causes a SQL error
      "Cannot create new connection because in manual or
      distributed transaction mode". Workaround:

      poFetchDetailsOnDemand = False (default)



dbExpress MSSQL driver BLOB access
-----------------------------------

dbExpress MSSQL uses ISequentialStream (OLEDB
interface) to access blobs from SQL Server. MS OLEDB
provider (SQLOLEDB) doesn't support accessing more than
one in a SELECT using ISequentialStream. So, if you are
projecting more than one blob column in your queries
the dbExpress driver will not use the ISequentialStream
to access blobs. Instead, it will bind huge buffers to
get the blob data. The buffer size is determined by the
BlobSize entry in dbxconnections.ini. Make sure to set
it to an appropriate size to improve performance when
more than one blob column is projected.


MySQL limitation (dbExpress)
----------------------------

MySQL can't filter with FLOAT fields in the WHERE
clause. Since this limitation prevents updates with
TClientDataset and TDataSetProvider, use DOUBLE
instead. For more information, see the MySQL
documentation.


Huge Text in MySQL (dbExpress)
------------------------------

MySQL Server has a startup parameter called max-
allowed-packet that, by default, is set to 1 megabyte
for most MySQL servers. Increase the value of max-
allowed-packet if you encounter difficulty writing
large amounts of data (such as large strings) to MySQL.


Oracle notes for dbExpress
--------------------------

When qualifying a table name with a user name, you must
use quotes around the table name when CommandType is
set to ctTable.

Oracle LONG and LONG RAW fields (dbExpress)
-------------------------------------------

When using Oracle, the size of LONG and LONG RAW fields
cannot be determined without fetching the whole field.
So if BLOB SIZE is set to -1, LONG and LONG RAW fields
are truncated to 64K. Recommendation: Set BLOB SIZE to
your best estimate of the blob size.

Configuring driver files  (dbExpress)
-------------------------------------
The Oracle driver now supports Trim Char, Multiple
Transaction and OS Authentication. You need to add
these settings to your dbxdrivers for new aliases
and/or update dbxconnections in order to include these
settings for old aliases.


Please append the following at the end of dbxdrivers
file if you are using a driver file from an earlier
version of Delphi:

    [Multiple Transaction]
    False=0
    True=1

    [Trim Char]
    False=0
    True=1

    [OS Authentication]
    False=0
    True=1

Add the following under the [Oracle] section in
dbxdrivers and under your existent Oracle alias.

    OS Authentication=False
    Multiple Transaction=False
    Trim Char=False


Oracle OS authentication  (dbExpress)
-------------------------------------

To get the correct list of tables and other schema
objects in the dbExpress components, set the User_Name
to OPS$XXX, where XXX is the OS Authenticated user.


=======================================================
ABOUT EDITING THE WINDOWS REGISTRY

Warning:
    Registry edits are performed at your own risk.
    Several items in this document and in other parts
    of the documentation that accompany this product
    describe how to edit your Windows registry. In most
    cases, we recommend that you use the Microsoft
    Regedit tool to perform your editing. Whatever tool
    you use, however, you should, before making any
    edits, make backups of your registry and thoroughly
    familiarize yourself with the editing tool and the
    registry editing process. The registry is a
    critical component of your operating system, and
    any mistakes made while editing it can cause your
    machine to stop functioning.


=======================================================
PRODUCT INFORMATION ON THE WEB

In addition to numerous private sites that offer
information, instruction, and samples, the following
Borland Web-based resources provide a continuous stream
of news, product information, updates, code, and other
materials. You can connect to many of these resources
directly from the Help menu.

Delphi Direct [Help|Delphi Direct]
----------------------------------

This IDE add-in resource provides an automatically
updated list of links to the latest news, downloads,
and other information about Delphi and Borland.

Code Central
------------

The CodeCentral Repository is a free, searchable
database of code, tips, and other materials of interest
to developers. For details and registration
information, visit

 http://codecentral.borland.com/


The Borland Web site
--------------------

Borland Software Corporation home page:
  http://www.borland.com
  [IDE: Help|Borland Home Page]

Delphi home page:
  http://www.borland.com/delphi/
  [IDE: Help|Delphi Home Page]

Delphi developer support:
  http://www.borland.com/devsupport/delphi/
  [IDE: Help|Delphi Developer Support]

Delphi updates and other downloads:
  http://www.borland.com/devsupport/delphi/downloads/

Delphi documentation site:
  http://www.borland.com/techpubs/delphi

Newsgroups:
  http://www.borland.com/newsgroups/

Worldwide offices and distributors:
  http://www.borland.com/bww/

FTP site (anonymous access):
  ftp.borland.com


=======================================================
Copyright (c) 2002 Borland Software Corporation.
All rights reserved.
