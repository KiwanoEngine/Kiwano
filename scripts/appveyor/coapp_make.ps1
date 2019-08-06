Write-Host "Start to build nupkg files"

# This is the CoApp .autopkg file to create.
$autopkgFile = "scripts\coapp\kiwano.autopkg"

# Get the ".autopkg.template" file, replace "@appveyor_version" with the Appveyor version number, then save to the ".autopkg" file.
Get-Content ($autopkgFile + ".template") | ForEach-Object { $_ -replace "@appveyor_version", $env:appveyor_build_version } > $autopkgFile

# Use the CoApp tools to create NuGet native packages from the .autopkg.
Write-NuGetPackage $autopkgFile

# Push all newly created .nupkg files as Appveyor artifacts for later deployment.
Get-ChildItem .\*.nupkg | ForEach-Object { Push-AppveyorArtifact $_.FullName -FileName $_.Name }