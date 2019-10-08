function Set-FileConfiguration {
    param(
        [string]$filePath
    )

    $replace1 = "<DebugInformationFormat>EditAndContinue</DebugInformationFormat>"
    $replace2 = "<DebugInformationFormat>ProgramDatabase</DebugInformationFormat>"
    $replaceTo = "<DebugInformationFormat>None</DebugInformationFormat>"

    # Create a copy of .vcxproj file
    Copy-Item -Path $filePath -Destination ($filePath + '.template')

    # Overlay some configurations
    Get-Content ($filePath + '.template') -Encoding UTF8 | ForEach-Object { $_ -replace $replace1, $replaceTo; $_ -replace $replace2, $replaceTo; } | Out-File $filePath -Encoding UTF8

    # Delete the copy file
    Remove-Item -Path ($filePath + '.template')
}

Get-ChildItem -Path 'projects\' -Directory | ForEach-Object {
    $dirPath = "projects\$($_)"

    # Search all vcxproj files
    Get-ChildItem -Path $dirPath *.vcxproj -File | ForEach-Object {
        Set-FileConfiguration $dirPath + $_
    }
}
