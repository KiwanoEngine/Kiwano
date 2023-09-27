function Copy-Recurse {
    param(
        [string]$Path,
        [string]$Destination,
        [string]$Filter
    )

    New-Item -ItemType Directory -Force -Path $Destination | Out-Null

    Get-ChildItem -Recurse -Path $Path -Filter $Filter | ForEach-Object {
        $relativePath = $_.FullName.Replace((Resolve-Path $Path).Path, "")
        $targetFile = Join-Path -Path $Destination -ChildPath $relativePath

        $targetFileDirectory = Split-Path -Path $targetFile -Parent
        
        if (!(Test-Path $targetFileDirectory))
        {
            New-Item -ItemType Directory -Force -Path $targetFileDirectory | Out-Null
        }
        
        Copy-Item -Path $_.FullName -Destination $targetFile -Force
    }
}
