$replace = "<DebugInformationFormat>EditAndContinue</DebugInformationFormat>"
$replaceTo = "<DebugInformationFormat>None</DebugInformationFormat>"

Get-ChildItem -Path 'projects\' *.vcxproj | ForEach-Object {
    $filePath = 'projects\' + $_

    # Create a copy of .vcxproj file
    Copy-Item -Path $filePath -Destination ($filePath + '.template')

    # Overlay some configurations
    Get-Content ($filePath + '.template') -Encoding UTF8 | ForEach-Object { $_ -replace $replace, $replaceTo } | Out-File $filePath -Encoding UTF8

    # Delete the copy file
    Remove-Item -Path ($filePath + '.template')
}
