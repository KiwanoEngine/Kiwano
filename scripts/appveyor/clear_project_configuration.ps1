$replace = "<DebugInformationFormat>EditAndContinue</DebugInformationFormat>"
$replaceTo = "<DebugInformationFormat>None</DebugInformationFormat>"

Get-ChildItem -Path 'projects\' *.vcxproj | ForEach-Object {
    $filePath = 'projects\' + $_
    Copy-Item -Path $filePath -Destination ($filePath + '.template')
    Get-Content ($filePath + '.template') | ForEach-Object { $_ -replace $replace, $replaceTo } > $filePath
}
