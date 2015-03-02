;;;;
;;;; Inno Setup Script
;;;; Interface of Nature (ION) 4.2.0
;;;;

[Setup]
AppId = {{9F03A84F-23C5-47BB-9BCC-EB9AF48EE96C}
AppName = "Interface of Nature"
AppVersion = "4.2.0"
AppPublisher = "Mark Karpov"
AppPublisherURL = "http://github.com/mrkkrp/ion"
AppSupportURL = "http://github.com/mrkkrp/ion"
AppUpdatesURL = "http://github.com/mrkkrp/ion"
DefaultDirName = "{pf}\Interface of Nature"
DefaultGroupName = "Interface of Nature"
LicenseFile = "LICENSE.md"
InfoBeforeFile = "README.md"
OutputDir = "install"
OutputBaseFilename = "ion-4.2.0-win-x86"
SetupIconFile = "resources\icons\install.ico"
Compression = "lzma"
SolidCompression = yes
ChangesAssociations = yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "bar\*";     DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\ION Launcher";           Filename: "{app}\bin\ion.exe"
Name: "{group}\ION Scores Editor";      Filename: "{app}\bin\scr.exe"
Name: "{group}\ION Generator";          Filename: "{app}\bin\gnr.exe"
Name: "{group}\ION Sonorities Editor";  Filename: "{app}\bin\snr.exe"
Name: "{group}\Parameter Lists Editor"; Filename: "{app}\bin\prl.exe"
Name: "{group}\Wave Master";            Filename: "{app}\bin\wm.exe"
Name: "{commondesktop}\ION Launcher";   Filename: "{app}\bin\ion.exe"; Tasks: desktopicon
Name: "{group}\Uninstall ION Package";  Filename: "{uninstallexe}"

[Run]
Filename: "{app}\bin\ion.exe"; Description: "{cm:LaunchProgram,Interface of Nature}"; Flags: nowait postinstall skipifsilent

[Registry]

Root: HKCR; Subkey: ".prl";                           ValueType: string; ValueData: "prlfile";                    Flags: uninsdeletekey
Root: HKCR; Subkey: "prlfile\shell\open\command";     ValueType: string; ValueData: "{app}\bin\prl.exe ""%1""";   Flags: uninsdeletekey
Root: HKCR; Subkey: "prlfile";                        ValueType: string; ValueData: "Parameter List";             Flags: uninsdeletekey
Root: HKCR; Subkey: "prlfile\DefaultIcon";            ValueType: string; ValueData: "{app}\prldoc.ico";           Flags: uninsdeletekey

Root: HKCR; Subkey: ".ionscr";                        ValueType: string; ValueData: "ionscrfile";                 Flags: uninsdeletekey
Root: HKCR; Subkey: "ionscrfile\shell\open\command";  ValueType: string; ValueData: "{app}\bin\scr.exe ""%1""";   Flags: uninsdeletekey
Root: HKCR; Subkey: "ionscrfile";                     ValueType: string; ValueData: "ION Score";                  Flags: uninsdeletekey
Root: HKCR; Subkey: "ionscrfile\DefaultIcon";         ValueType: string; ValueData: "{app}\scrdoc.ico";           Flags: uninsdeletekey

Root: HKCR; Subkey: ".ionsnr";                        ValueType: string; ValueData: "ionsnrfile";                 Flags: uninsdeletekey
Root: HKCR; Subkey: "ionsnrfile\shell\open\command";  ValueType: string; ValueData: "{app}\bin\snr.exe ""%1""";   Flags: uninsdeletekey
Root: HKCR; Subkey: "ionsnrfile";                     ValueType: string; ValueData: "ION Sonority";               Flags: uninsdeletekey
Root: HKCR; Subkey: "ionsnrfile\DefaultIcon";         ValueType: string; ValueData: "{app}\snrdoc.ico";           Flags: uninsdeletekey

Root: HKCR; Subkey: ".iongnr";                        ValueType: string; ValueData: "iongnrfile";                 Flags: uninsdeletekey
Root: HKCR; Subkey: "iongnrfile\shell\open\command";  ValueType: string; ValueData: "{app}\bin\gnr.exe ""%1""";   Flags: uninsdeletekey
Root: HKCR; Subkey: "iongnrfile";                     ValueType: string; ValueData: "ION Generator";              Flags: uninsdeletekey
Root: HKCR; Subkey: "iongnrfile\DefaultIcon";         ValueType: string; ValueData: "{app}\gnrdoc.ico";           Flags: uninsdeletekey
