[linha: 1]                     Delphi 7
[linha: 2]                  Release Notes
[linha: 3] 
[linha: 4] 
[linha: 5] =======================================================
[linha: 6] 
[linha: 7] This file contains important supplementary and late-
[linha: 8] breaking information that may not appear in the main
[linha: 9] product documentation. We recommend that you read this
[linha: 10] file in its entirety.
[linha: 11] 
[linha: 12] For information on new features in this release, choose
[linha: 13] "What's New" in the online Help contents.
[linha: 14] 
[linha: 15] 
[linha: 16] =======================================================
[linha: 17] CONTENTS
[linha: 18] 
[linha: 19] * OTHER RELEASE NOTES INCLUDED WITH THIS PRODUCT
[linha: 20] * NOTES AND ISSUES:
[linha: 21] * ABOUT EDITING THE REGISTRY
[linha: 22] * PRODUCT INFORMATION ON THE WEB
[linha: 23] 
[linha: 24] 
[linha: 25] =======================================================
[linha: 26] OTHER RELEASE NOTES INCLUDED WITH THIS PRODUCT
[linha: 27] 
[linha: 28] * INSTALL contains system requirements and product
[linha: 29]   installation information.
[linha: 30] 
[linha: 31] * DEPLOY contains information about redistributing your
[linha: 32]   applications.
[linha: 33] 
[linha: 34] * LICENSE contains information on licensing allowances
[linha: 35]   and limitations for this product and other Borland
[linha: 36]   software that is bundled with it.
[linha: 37] 
[linha: 38]     The three files listed above, along with this file,
[linha: 39]     are installed in your main product directory
[linha: 40]     (default: C:\Program Files\Borland\Delphi7).
[linha: 41] 
[linha: 42] =======================================================
[linha: 43] NOTES AND ISSUES
[linha: 44] 
[linha: 45] Deprecated Components
[linha: 46] 
[linha: 47]     Borland is deprecating the use of TSQLClientDataSet
[linha: 48]     and TBDEClientDataSet. Borland recommends that you
[linha: 49]     use TSimpleDataSet for simple, two-tier
[linha: 50]     applications. TSQLClientDataSet and
[linha: 51]     TBDEClientDataSet are no longer displayed on the
[linha: 52]     Component palette. If you require backward
[linha: 53]     compatibility, you can still access
[linha: 54]     TSQLClientDataSet and TBDEClientDataSet in the
[linha: 55]     Demos directory (by default, C:\Program
[linha: 56]     Files\Borland\Delphi7\Demos).
[linha: 57] 
[linha: 58] 
[linha: 59]     Borland is deprecating the use of the TServerSocket
[linha: 60]     and TClientSocket from the unit ScktComp. It is
[linha: 61]     recommended that you use the Indy components for
[linha: 62]     socket operations. The TServerSocket and
[linha: 63]     TClientSocket will no longer be installed on the
[linha: 64]     component palette by default. If you require the
[linha: 65]     use of these components then you can install
[linha: 66]     the design time package named dclsockets70.bpl,
[linha: 67]     found in your bin directory. For deployment with
[linha: 68]     runtime packages, you will need to deploy rtl70.bpl
[linha: 69]     and any other required packages.
[linha: 70] 
[linha: 71] 
[linha: 72] Changes in StrUtils
[linha: 73] -------------------
[linha: 74] 
[linha: 75] The StrUtils unit contains three sets of changes, all
[linha: 76] relating to
[linha: 77] multi-byte character set (MBCS) support.
[linha: 78] 
[linha: 79]    * Previously, LeftStr, RightStr, and MidStr each
[linha: 80]      took and returned AnsiString values, and did not
[linha: 81]      support MBCS strings. Each of these functions has
[linha: 82]      been replaced by a pair of overloaded functions,
[linha: 83]      one that takes and returns AnsiString, and one
[linha: 84]      that takes and returns WideString. The new
[linha: 85]      functions correctly handle MBCS strings. This
[linha: 86]      change breaks code that uses these functions to
[linha: 87]      store and retrieve byte values in AnsiStrings.
[linha: 88]      Such code should use the new byte-level functions
[linha: 89]      described below.
[linha: 90] 
[linha: 91]    * New functions LeftBStr, RightBStr, and MidBStr
[linha: 92]      provide the byte-level manipulation previously
[linha: 93]      provided by LeftStr, RightStr, and MidStr.
[linha: 94] 
[linha: 95]    * New functions AnsiLeftStr, AnsiRightStr, and
[linha: 96]      AnsiMidStr are the same as the new AnsiStr
[linha: 97]      LeftStr, RightStr, and MidStr functions, except
[linha: 98]      that they are not overloaded with equivalent
[linha: 99]      WideString functions.
[linha: 100] 
[linha: 101] 
[linha: 102] XP Themes
[linha: 103] ---------
[linha: 104] 
[linha: 105] This build does not include a manifest for your
[linha: 106] projects. To tell your applications to use version 6.0
[linha: 107] of comctl32.dll you need to create and edit the
[linha: 108] manifest yourself.
[linha: 109] 
[linha: 110] There are two ways to do this:
[linha: 111] 
[linha: 112] 1. With a stand-alone manifest file added to the same
[linha: 113]    directory of the executable.
[linha: 114] 
[linha: 115] 2. With a manifest included in the executable as a
[linha: 116]    resource.
[linha: 117] 
[linha: 118] 
[linha: 119] For the stand-alone manifest follow these steps:
[linha: 120] 
[linha: 121] I) Using Notepad create a file called
[linha: 122]    "Project1.exe.manifest" and add the following lines:
[linha: 123] 
[linha: 124] <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
[linha: 125] <assembly xmlns="urn:schemas-microsoft-com:asm.v1"
[linha: 126] manifestVersion="1.0">
[linha: 127] <assemblyIdentity
[linha: 128] processorArchitecture="*"
[linha: 129] version="1.0.0.0"
[linha: 130] type="win32"
[linha: 131] name="Project1"/>
[linha: 132] <description>Project1 Description</description>
[linha: 133] <dependency>
[linha: 134] <dependentAssembly>
[linha: 135] <assemblyIdentity
[linha: 136] type="win32"
[linha: 137] name="Microsoft.Windows.Common-Controls"
[linha: 138] version="6.0.0.0"
[linha: 139] publicKeyToken="6595b64144ccf1df"
[linha: 140] language="*"
[linha: 141] processorArchitecture="*"   />
[linha: 142] </dependentAssembly>
[linha: 143] </dependency>
[linha: 144] </assembly>
[linha: 145] 
[linha: 146] II) Save the file and place it in the same directory as
[linha: 147]     the application executable.
[linha: 148] 
[linha: 149] If you don't like the idea of having the manifest as a
[linha: 150] separated file you can include it into your executable
[linha: 151] as a resource.
[linha: 152] 
[linha: 153] I) Create a resource file (again using Notepad)
[linha: 154]    containing the following line:
[linha: 155] 
[linha: 156]    1 24 "Project1.exe.Manifest"
[linha: 157] 
[linha: 158] II) Compile the resource file with brcc32.exe with the
[linha: 159]     following command:
[linha: 160] 
[linha: 161]    C:\Project1> brcc32 WindowsXP.RC
[linha: 162] 
[linha: 163] III) Now add the following line to your project's main
[linha: 164]      form:
[linha: 165] 
[linha: 166]    {$R WindowsXP.RES}
[linha: 167] 
[linha: 168] 
[linha: 169] NOTE: To automatically configure XP Themes in your
[linha: 170]       project you can use the XP Application Wizard
[linha: 171]       demo. For more information see the readme file in
[linha: 172] 
[linha: 173]        Demos\ToolsAPI\XPAppWiz
[linha: 174] 
[linha: 175] 
[linha: 176] .NET compiler warnings
[linha: 177] ----------------------
[linha: 178] 
[linha: 179] The Delphi 7 dcc32 compiler now supports three
[linha: 180] additional compiler warnings:  Unsafe_Type,
[linha: 181] Unsafe_Code, and Unsafe_Cast.  These warnings are
[linha: 182] disabled by default, but can be enabled with source
[linha: 183] code compiler directives {$WARN UNSAFE_CODE ON},
[linha: 184] compiler command line switches (dcc32 -W+UNSAFE_CODE),
[linha: 185] and in Project|Options.
[linha: 186] 
[linha: 187] NOTE: There is no space before or after the "+" in the
[linha: 188]       command line switch)
[linha: 189] 
[linha: 190] "Unsafe" in all three messages refers to types or
[linha: 191] operations which static code analysis cannot prove to
[linha: 192] not overwrite memory.  For example, data types that
[linha: 193] carry bounds information at runtime such as Delphi
[linha: 194] Strings or dynamic arrays can be bounds-checked at
[linha: 195] runtime, and tracked in static analysis, to ensure that
[linha: 196] memory accesses are within the limits of the allocated
[linha: 197] data.  Data types that do not carry bounds information
[linha: 198] at compile time or runtime (such as PChar) cannot be
[linha: 199] proven safe.  Unsafe doesn't necessarily mean the code
[linha: 200] is broken or flawed, it simply means that it uses
[linha: 201] programming techniques that cannot be verified as safe
[linha: 202] by static code analysis.  In a secured execution
[linha: 203] environment such as .NET, such code is assumed to be
[linha: 204] unsafe and a potential security risk.
[linha: 205] 
[linha: 206] Delphi 7 developers can use these new warnings to
[linha: 207] identify data types, code or programming techniques
[linha: 208] that may be difficult to port to a managed code
[linha: 209] environment such as .NET.  These messages are not
[linha: 210] specific to the .NET platform.  The warnings are
[linha: 211] conservative - some things we warn about in D7 we might
[linha: 212] actually be able to support in Delphi for .NET, but
[linha: 213] we're not sure yet.  The warnings are not complete -
[linha: 214] there may be problematic types or code situations for
[linha: 215] which the Delphi 7 compiler does not issue an unsafe
[linha: 216] warning.
[linha: 217] 
[linha: 218] Unsafe_Type:  Types such as PChar, untyped pointer,
[linha: 219] untyped var and out parameters, file of <type>, 6 byte
[linha: 220] reals (Real48), variant records (records containing
[linha: 221] overlapping fields) and old-style objects
[linha: 222] 
[linha: 223] ("TMyObject = object").
[linha: 224] 
[linha: 225]   * "old" object type
[linha: 226] 
[linha: 227]        c = object
[linha: 228]          i: Integer;
[linha: 229]          procedure p;
[linha: 230]        end;
[linha: 231] 
[linha: 232] Unsafe_Code:  absolute variables, Addr(), Ptr(), Hi(),
[linha: 233] Lo(), Swap() standard procedures, BlockRead and
[linha: 234] BlockWrite, the Fail() standard procedure, GetMem(),
[linha: 235] FreeMem(), and ReallocMem()
[linha: 236] 
[linha: 237]   * inline assembler blocks (asm end)
[linha: 238]   * @ operator
[linha: 239]   * modifying string index elements, e.g
[linha: 240] 
[linha: 241]       s := 'Hoho';
[linha: 242]       s[2] := 'a'; // <-- "Unsafe code 'String index to
[linha: 243]       var param'"
[linha: 244] 
[linha: 245] Unsafe_Cast:  Casting an object instance to a type that
[linha: 246] is not an ancestor or descendent of the instance type,
[linha: 247] casting a record type to anything else
[linha: 248] 
[linha: 249] 
[linha: 250] Change to VCL SubComponents streaming
[linha: 251] -------------------------------------
[linha: 252] 
[linha: 253] In Delphi 7, we corrected a problem where the csLoading
[linha: 254] flag was never set for subcomponents, nor was the
[linha: 255] subcomponent's Loaded method called. When a component t
[linha: 256] at
[linha: 257] has subcomponents is streamed, the subcomponents will h
[linha: 258] ve
[linha: 259] their csLoading flag set and their Loaded method called
[linha: 260] 
[linha: 261] This change creates a complication for any subcomponent
[linha: 262] properties that are writable. If you allow your
[linha: 263] subcomponent property to be assigned to an external
[linha: 264] component reference then you cannot free your subcompon
[linha: 265] nt
[linha: 266] until it's owner's Loaded method is called otherwise th
[linha: 267] 
[linha: 268] streaming system will attempt to call the subcomponent'
[linha: 269] 
[linha: 270] Loaded method after the subcomponent has been freed.
[linha: 271] 
[linha: 272] Apache
[linha: 273] ------
[linha: 274] 
[linha: 275] Support for Apache 2 is for the 2.0.39 version. It will
[linha: 276] work with later versions as long as they are binary
[linha: 277] compatible.
[linha: 278] 
[linha: 279] NOTE: Due to a known bug with Apache 1.3.22, do not
[linha: 280]       develop CGI programs or shared objects for this
[linha: 281]       version.
[linha: 282] 
[linha: 283]       See http://bugs.apache.org/index.cgi/full/8538
[linha: 284] 
[linha: 285] 
[linha: 286] UDDI Registry entries
[linha: 287] ---------------------
[linha: 288] 
[linha: 289] Some UDDI Registry entries contain an Accesspoint URL w
[linha: 290] th
[linha: 291] a bookmark tag at the end. When accessed these services
[linha: 292] often generate the error message 'Method not allowed
[linha: 293] (405)'.
[linha: 294] 
[linha: 295] Workaround:
[linha: 296] 
[linha: 297] Remove the tag from url in the global function
[linha: 298] that returns the Interface:
[linha: 299] 
[linha: 300]      const
[linha: 301] //        defURL = 'http://someservice/somewhere#tag';
[linha: 302] //        is changed to
[linha: 303] 
[linha: 304]         defURL = 'http://someservice/somewhere';
[linha: 305] 
[linha: 306] 
[linha: 307] Japanese Input system on Windows XP
[linha: 308] -----------------------------------
[linha: 309] 
[linha: 310] The MS IME2002 Japanese input system included with
[linha: 311] Windows XP can cause the Debugger in the IDE to
[linha: 312] stall.
[linha: 313] 
[linha: 314] Workaround:
[linha: 315] 
[linha: 316] Remove Microsoft IME Character Code Dictionary from
[linha: 317] your system dictionaries list. Simply un-checking this
[linha: 318] option from the dictionary list will not solve the
[linha: 319] problem. To remove this dictionary:
[linha: 320] 
[linha: 321] 1) Open the IME Toolbar.
[linha: 322] 2) Select Tools|Properties and click on the Dictionary
[linha: 323]    tab.
[linha: 324] 3) Select "Microsoft IME Character Code Dictionary"
[linha: 325]    from the System dictionaries listbox
[linha: 326] 4) Click Remove.
[linha: 327] 
[linha: 328] 
[linha: 329] Database
[linha: 330] --------
[linha: 331] * The provided DB2 driver is certified for DB2 version
[linha: 332]   7 only. The client version and server version must
[linha: 333]   match. BDE also supports DB2 version 7 only.
[linha: 334] 
[linha: 335] * The Oracle driver provided for dbExpress components
[linha: 336]   is certified for Oracle 9i. BDE and ADO have been
[linha: 337]   certified with Oracle 8.1.7 and 8.1.6. In all cases,
[linha: 338]   the client version and server version must be the
[linha: 339]   same.
[linha: 340] 
[linha: 341] * Oracle 9i types - ORACLE 9i types TIMESTAMP
[linha: 342]   (introduced for JDK 1.3.1 compliance) and other types
[linha: 343]   like XMLType, Temp tables, etc. are not yet
[linha: 344]   supported. Use "DATE" instead.
[linha: 345] 
[linha: 346] * The Borland Database Engine CAB file (BDEINST.CAB) is
[linha: 347]   no longer digitally signed. This CAB file is provided
[linha: 348]   for backward compatibility. For new applications, we
[linha: 349]   recommend deploying BDE with the BDE merge modules.
[linha: 350] 
[linha: 351] InterBase version support, Dialect 3 features
[linha: 352] ---------------------------------------------
[linha: 353] 
[linha: 354] Delphi 7 supports InterBase 6.5. The client version and
[linha: 355] server version must be the same.
[linha: 356] 
[linha: 357] Currently, the driver does not support the ARRAY type.
[linha: 358] 
[linha: 359] To use the new InterBase 6.5 Dialect 3 features in BDE,
[linha: 360] add an entry to your Windows registry under
[linha: 361] 
[linha: 362]  HKEY_LOCAL_MACHINE\SOFTWARE\Borland\Database Engine
[linha: 363]  \Settings\Drivers\Intrbase\Db Open\SQLDIALECT
[linha: 364] 
[linha: 365] and set the String Value to "3".
[linha: 366] 
[linha: 367] When an InterBase alias is created, the new entry will
[linha: 368] be available in the .CFG file.
[linha: 369] 
[linha: 370] To use InterBase 5.6, SQLDIALECT can be set to "1"
[linha: 371] (existing IB aliases which do not have the SQLDIALECT
[linha: 372] entry default to SQLDIALECT=1 or to the registry
[linha: 373] setting when the SQLDIALECT entry is added to the
[linha: 374] registry).
[linha: 375] 
[linha: 376] 
[linha: 377] InterBase 6.5 implementation note
[linha: 378] ---------------------------------
[linha: 379] 
[linha: 380] If you are using a WebSnap application with InterBase
[linha: 381] 6.5, a local database connection is not recommended.
[linha: 382] Instead, use a client/server connection specified by a
[linha: 383] protocol-specific prefix to the path to the database.
[linha: 384] 
[linha: 385] Examples:
[linha: 386] 
[linha: 387] Local connection (not recommended):
[linha: 388]     C:\PathTo\Database\DBName.gdb
[linha: 389] 
[linha: 390] TCP/IP connection on Windows:
[linha: 391]     saturn:C:\PathTo\Database\DBName.gdb
[linha: 392] 
[linha: 393] TCP/IP connection on UNIX:
[linha: 394]     jupiter:/usr/PathTo/Database/DBName.gdb
[linha: 395] 
[linha: 396] NetBEUI connection:
[linha: 397]     \\venus\C:\PathTo\Database\DBName.gdb
[linha: 398] 
[linha: 399] IPX/SPX connection:
[linha: 400]     mars@vol2:\PathTo\Database\DBName.gdb
[linha: 401] 
[linha: 402] If the WebSnap application is on the same machine as
[linha: 403] the server, you can use localhost:
[linha: 404] 
[linha: 405]     localhost:C:\PathTo\Database\DBName.gdb
[linha: 406] 
[linha: 407] 
[linha: 408] INTERBASE driver behavioral difference:
[linha: 409] ---------------------------------------
[linha: 410] 
[linha: 411] NUMERIC data type mapping.
[linha: 412] 
[linha: 413] 
[linha: 414]    The dbExpress driver for INTERBASE  now maps NUMERIC
[linha: 415]    data types to fldBCD instead of fldINT16, fldINT32,
[linha: 416]    fldFLOAT.
[linha: 417] 
[linha: 418] 
[linha: 419] dbExpress
[linha: 420] ---------
[linha: 421] 
[linha: 422] TSQLDataSet now has DefaultRowsetSize = 20 (Oracle
[linha: 423] only). To use a different RowSetSize, add the
[linha: 424] RowSetSize property manually (e.g., "RowsetSize = 200")
[linha: 425] into dbxconnections.ini, for existing connections, or
[linha: 426] into dbxdrivers.ini to have the RowSetSize property
[linha: 427] included in new connections. RowsetSize can also be
[linha: 428] modified in code, as shown here:
[linha: 429] 
[linha: 430]     "SQLConnection1.Params.Values['RowsetSize'] :=
[linha: 431]     '200'"
[linha: 432] 
[linha: 433] 
[linha: 434] dbExpress now includes MySQL 3.23.49 support with a new
[linha: 435] driver (dbexpmysql.dll). To use the new driver, specify
[linha: 436] 
[linha: 437]     LibraryName = "dbexpmysql.dll"
[linha: 438] 
[linha: 439] in the Object Inspector or in dbxdrivers.ini.
[linha: 440] 
[linha: 441] 
[linha: 442] dbExpress components and MySQL transactions
[linha: 443] -------------------------------------------
[linha: 444] 
[linha: 445] MySQL doesn't support more than one active statement
[linha: 446] per connection. To perform multiple SQL requests with a
[linha: 447] single TSQLConnection the dbExpress components clone
[linha: 448] connection. There is a bug in maintaining state
[linha: 449] information on the no.of active statements with respect
[linha: 450] to a TSQLConnection. This might cause serious failures
[linha: 451] in transaction as the transaction might be started on
[linha: 452] one connection and commit or rollback might be
[linha: 453] happening on another connection.
[linha: 454] 
[linha: 455] Workaround:
[linha: 456] 
[linha: 457] Before you start a MySQL transaction make sure
[linha: 458] TSQLConnection.ActiveStatements is 0. If it's not 0 the
[linha: 459] 
[linha: 460] close the TSQLConnection and reopen it and that should
[linha: 461] clear it. All your transaction activity should then go
[linha: 462] on a single connection.
[linha: 463] 
[linha: 464] DB2 notes for dbExpress
[linha: 465] -----------------------
[linha: 466] 
[linha: 467] The provided DB2 driver is certified for DB2 version 7.
[linha: 468] The client version and server version must be the
[linha: 469] same. BDE also supports DB2 version 7.
[linha: 470] 
[linha: 471] Because of a known DB2 bug, the DB2 client returns only
[linha: 472] one record at a time when a request for a block fetch
[linha: 473] is made, even with RowsetSize option > 1.
[linha: 474] 
[linha: 475] Timestamp issues
[linha: 476] ----------------
[linha: 477] 
[linha: 478] The dbExpress DB2 driver rounds the TIMESTAMP
[linha: 479] fractional value and there could be some precision lost
[linha: 480] depending upon the data retrieved. When you apply back
[linha: 481] changes the old value will not match the one on the
[linha: 482] server and ApplyUpdates() will fail.
[linha: 483] 
[linha: 484] Workaround:
[linha: 485] 
[linha: 486] Make sure DB2 TIMESTAMP is not part of indexed columns
[linha: 487] and set the DataSetProvider Update mode to WhereKey
[linha: 488] instead of WhereAll.
[linha: 489] 
[linha: 490] 
[linha: 491] Informix BLOB/CLOB fields (dbExpress)
[linha: 492] -------------------------------------
[linha: 493] 
[linha: 494] A new global variable, InformixLob, is available to
[linha: 495] allow you to work with Informix BLOB and CLOB fields.
[linha: 496] 
[linha: 497] The variable is required because Informix BLOB
[linha: 498] (fldBLOB, fldstHBINARY) and CLOB (fldBLOB, fldstHMEMO)
[linha: 499] fields are mapped as ftOraBlob and ftOraClob,
[linha: 500] respectively, and the DataSnap resolver performs
[linha: 501] special query generation when resolving Oracle
[linha: 502] BLOB/CLOB fields, but not when resolving Informix
[linha: 503] BLOB/CLOBs. To address this issue, set InformixLob
[linha: 504] to True when using Informix BLOB/CLOBs, and switch it
[linha: 505] back to False (the default) when using Oracle
[linha: 506] BLOB/CLOBs.
[linha: 507] 
[linha: 508] 
[linha: 509] NOTE: The new Informix driver (dbexpinf.dll) has been
[linha: 510]       tested with Informix version 9.2.1 client/server
[linha: 511]       and 9.2.1 client with SE server.
[linha: 512] 
[linha: 513] MSSQL driver - Known issues
[linha: 514] ---------------------------
[linha: 515] 
[linha: 516] 1) Connection with a Blank Password/OS Authentication:
[linha: 517] 
[linha: 518]    With the current MSSQL driver you can't connect usin
[linha: 519]  a
[linha: 520]    blank password or with OS Authentication = True.
[linha: 521] 
[linha: 522]    A patch will be made available soon at Borland's sup
[linha: 523] ort
[linha: 524]    download site.
[linha: 525] 
[linha: 526] 
[linha: 527] 2) Master detail:
[linha: 528] 
[linha: 529]    a) When trying to open a master-detail as a nested
[linha: 530]       dataset and if the detail link field is an INTEGE
[linha: 531] 
[linha: 532]       type and defined NOT NULL you will get a SQL Erro
[linha: 533] 
[linha: 534]       "SQL State:HY000, SQL Error Code: 0, Invalid Inpu
[linha: 535] 
[linha: 536]       parameter values. Check the status values for
[linha: 537]       details"
[linha: 538] 
[linha: 539]       A patch will be made available soon at Borland's
[linha: 540]       support download site.
[linha: 541] 
[linha: 542]    b) ApplyUpdates() on a master-detail with
[linha: 543]       "poFetchDetailsOnDemand" = True causes a SQL erro
[linha: 544] 
[linha: 545]       "Cannot create new connection because in manual o
[linha: 546] 
[linha: 547]       distributed transaction mode". Workaround:
[linha: 548] 
[linha: 549]       poFetchDetailsOnDemand = False (default)
[linha: 550] 
[linha: 551] 
[linha: 552] 
[linha: 553] dbExpress MSSQL driver BLOB access
[linha: 554] -----------------------------------
[linha: 555] 
[linha: 556] dbExpress MSSQL uses ISequentialStream (OLEDB
[linha: 557] interface) to access blobs from SQL Server. MS OLEDB
[linha: 558] provider (SQLOLEDB) doesn't support accessing more than
[linha: 559] one in a SELECT using ISequentialStream. So, if you are
[linha: 560] projecting more than one blob column in your queries
[linha: 561] the dbExpress driver will not use the ISequentialStream
[linha: 562] to access blobs. Instead, it will bind huge buffers to
[linha: 563] get the blob data. The buffer size is determined by the
[linha: 564] BlobSize entry in dbxconnections.ini. Make sure to set
[linha: 565] it to an appropriate size to improve performance when
[linha: 566] more than one blob column is projected.
[linha: 567] 
[linha: 568] 
[linha: 569] MySQL limitation (dbExpress)
[linha: 570] ----------------------------
[linha: 571] 
[linha: 572] MySQL can't filter with FLOAT fields in the WHERE
[linha: 573] clause. Since this limitation prevents updates with
[linha: 574] TClientDataset and TDataSetProvider, use DOUBLE
[linha: 575] instead. For more information, see the MySQL
[linha: 576] documentation.
[linha: 577] 
[linha: 578] 
[linha: 579] Huge Text in MySQL (dbExpress)
[linha: 580] ------------------------------
[linha: 581] 
[linha: 582] MySQL Server has a startup parameter called max-
[linha: 583] allowed-packet that, by default, is set to 1 megabyte
[linha: 584] for most MySQL servers. Increase the value of max-
[linha: 585] allowed-packet if you encounter difficulty writing
[linha: 586] large amounts of data (such as large strings) to MySQL.
[linha: 587] 
[linha: 588] 
[linha: 589] Oracle notes for dbExpress
[linha: 590] --------------------------
[linha: 591] 
[linha: 592] When qualifying a table name with a user name, you must
[linha: 593] use quotes around the table name when CommandType is
[linha: 594] set to ctTable.
[linha: 595] 
[linha: 596] Oracle LONG and LONG RAW fields (dbExpress)
[linha: 597] -------------------------------------------
[linha: 598] 
[linha: 599] When using Oracle, the size of LONG and LONG RAW fields
[linha: 600] cannot be determined without fetching the whole field.
[linha: 601] So if BLOB SIZE is set to -1, LONG and LONG RAW fields
[linha: 602] are truncated to 64K. Recommendation: Set BLOB SIZE to
[linha: 603] your best estimate of the blob size.
[linha: 604] 
[linha: 605] Configuring driver files  (dbExpress)
[linha: 606] -------------------------------------
[linha: 607] The Oracle driver now supports Trim Char, Multiple
[linha: 608] Transaction and OS Authentication. You need to add
[linha: 609] these settings to your dbxdrivers for new aliases
[linha: 610] and/or update dbxconnections in order to include these
[linha: 611] settings for old aliases.
[linha: 612] 
[linha: 613] 
[linha: 614] Please append the following at the end of dbxdrivers
[linha: 615] file if you are using a driver file from an earlier
[linha: 616] version of Delphi:
[linha: 617] 
[linha: 618]     [Multiple Transaction]
[linha: 619]     False=0
[linha: 620]     True=1
[linha: 621] 
[linha: 622]     [Trim Char]
[linha: 623]     False=0
[linha: 624]     True=1
[linha: 625] 
[linha: 626]     [OS Authentication]
[linha: 627]     False=0
[linha: 628]     True=1
[linha: 629] 
[linha: 630] Add the following under the [Oracle] section in
[linha: 631] dbxdrivers and under your existent Oracle alias.
[linha: 632] 
[linha: 633]     OS Authentication=False
[linha: 634]     Multiple Transaction=False
[linha: 635]     Trim Char=False
[linha: 636] 
[linha: 637] 
[linha: 638] Oracle OS authentication  (dbExpress)
[linha: 639] -------------------------------------
[linha: 640] 
[linha: 641] To get the correct list of tables and other schema
[linha: 642] objects in the dbExpress components, set the User_Name
[linha: 643] to OPS$XXX, where XXX is the OS Authenticated user.
[linha: 644] 
[linha: 645] 
[linha: 646] =======================================================
[linha: 647] ABOUT EDITING THE WINDOWS REGISTRY
[linha: 648] 
[linha: 649] Warning:
[linha: 650]     Registry edits are performed at your own risk.
[linha: 651]     Several items in this document and in other parts
[linha: 652]     of the documentation that accompany this product
[linha: 653]     describe how to edit your Windows registry. In most
[linha: 654]     cases, we recommend that you use the Microsoft
[linha: 655]     Regedit tool to perform your editing. Whatever tool
[linha: 656]     you use, however, you should, before making any
[linha: 657]     edits, make backups of your registry and thoroughly
[linha: 658]     familiarize yourself with the editing tool and the
[linha: 659]     registry editing process. The registry is a
[linha: 660]     critical component of your operating system, and
[linha: 661]     any mistakes made while editing it can cause your
[linha: 662]     machine to stop functioning.
[linha: 663] 
[linha: 664] 
[linha: 665] =======================================================
[linha: 666] PRODUCT INFORMATION ON THE WEB
[linha: 667] 
[linha: 668] In addition to numerous private sites that offer
[linha: 669] information, instruction, and samples, the following
[linha: 670] Borland Web-based resources provide a continuous stream
[linha: 671] of news, product information, updates, code, and other
[linha: 672] materials. You can connect to many of these resources
[linha: 673] directly from the Help menu.
[linha: 674] 
[linha: 675] Delphi Direct [Help|Delphi Direct]
[linha: 676] ----------------------------------
[linha: 677] 
[linha: 678] This IDE add-in resource provides an automatically
[linha: 679] updated list of links to the latest news, downloads,
[linha: 680] and other information about Delphi and Borland.
[linha: 681] 
[linha: 682] Code Central
[linha: 683] ------------
[linha: 684] 
[linha: 685] The CodeCentral Repository is a free, searchable
[linha: 686] database of code, tips, and other materials of interest
[linha: 687] to developers. For details and registration
[linha: 688] information, visit
[linha: 689] 
[linha: 690]  http://codecentral.borland.com/
[linha: 691] 
[linha: 692] 
[linha: 693] The Borland Web site
[linha: 694] --------------------
[linha: 695] 
[linha: 696] Borland Software Corporation home page:
[linha: 697]   http://www.borland.com
[linha: 698]   [IDE: Help|Borland Home Page]
[linha: 699] 
[linha: 700] Delphi home page:
[linha: 701]   http://www.borland.com/delphi/
[linha: 702]   [IDE: Help|Delphi Home Page]
[linha: 703] 
[linha: 704] Delphi developer support:
[linha: 705]   http://www.borland.com/devsupport/delphi/
[linha: 706]   [IDE: Help|Delphi Developer Support]
[linha: 707] 
[linha: 708] Delphi updates and other downloads:
[linha: 709]   http://www.borland.com/devsupport/delphi/downloads/
[linha: 710] 
[linha: 711] Delphi documentation site:
[linha: 712]   http://www.borland.com/techpubs/delphi
[linha: 713] 
[linha: 714] Newsgroups:
[linha: 715]   http://www.borland.com/newsgroups/
[linha: 716] 
[linha: 717] Worldwide offices and distributors:
[linha: 718]   http://www.borland.com/bww/
[linha: 719] 
[linha: 720] FTP site (anonymous access):
[linha: 721]   ftp.borland.com
[linha: 722] 
[linha: 723] 
[linha: 724] =======================================================
[linha: 725] Copyright (c) 2002 Borland Software Corporation.
[linha: 726] All rights reserved.
