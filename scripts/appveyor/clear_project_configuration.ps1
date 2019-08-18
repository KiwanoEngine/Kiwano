$replace = "<DebugInformationFormat>EditAndContinue</DebugInformationFormat>"
$replaceTo = "<DebugInformationFormat>None</DebugInformationFormat>"

Get-ChildItem -Path 'projects\' *.vcxproj | ForEach-Object {
    $filePath = 'projects\' + $_
    Copy-Item -Path $filePath -Destination ($filePath + '.template')
    Get-Content ($filePath + '.template') -Encoding UTF8 | ForEach-Object { $_ -replace $replace, $replaceTo } | Out-File $filePath -Encoding UTF8
    Remove-Item -Path ($filePath + '.template')
}
