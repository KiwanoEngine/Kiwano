function Set-FileConfiguration {
    param(
        [string]$filePath
    )

    $debugInfoReplace = "<DebugInformationFormat>(EditAndContinue|ProgramDatabase)</DebugInformationFormat>"
    $debugInfoReplaceTo = "<DebugInformationFormat>None</DebugInformationFormat>"

    $optimizationReplace = "<WholeProgramOptimization>true</WholeProgramOptimization>"
    $optimizationReplaceTo = "<WholeProgramOptimization>false</WholeProgramOptimization>"

    # Create a copy of .vcxproj file
    Copy-Item -Path $filePath -Destination ($filePath + '.template')

    # Overlay some configurations
    Get-Content ($filePath + '.template') -Encoding UTF8 | ForEach-Object {
        ( $_ -replace $debugInfoReplace, $debugInfoReplaceTo ) -replace $optimizationReplace, $optimizationReplaceTo
    } | Out-File $filePath -Encoding UTF8

    # Delete the copy file
    Remove-Item -Path ($filePath + '.template')
}

Get-ChildItem -Path 'projects\' -Directory | ForEach-Object {
    $dirPath = "projects\$($_)"

    # Search all vcxproj files
    Get-ChildItem -Path $dirPath *.vcxproj -File | ForEach-Object {
        Set-FileConfiguration ($dirPath + '\' + $_)
    }
}

Get-ChildItem -Path 'projects\3rd-party' -Directory | ForEach-Object {
    $dirPath = "projects\3rd-party\$($_)"

    # Search all vcxproj files
    Get-ChildItem -Path $dirPath *.vcxproj -File | ForEach-Object {
        Set-FileConfiguration ($dirPath + '\' + $_)
    }
}
