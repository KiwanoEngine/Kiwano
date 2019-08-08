. .\scripts\appveyor\appveyor_get_build.ps1
. .\scripts\appveyor\appveyor_get_artifacts.ps1

# get job artifacts
Write-Host "Start to download artifacts from other jobs"

(Get-AppVeyorBuild).build.jobs | foreach-object {
    $jobId = $_.jobId
    if ($jobId -ne $env:APPVEYOR_JOB_ID) {
        # Get job artifacts information
        (Get-AppVeyorArtifacts -Job $jobId) | foreach-object {
            # Create directory if not exists
            $filePath = $_.fileName.Substring(0, $_.fileName.LastIndexOf('/'))
            if (!(Test-Path -Path $filePath)) {
                New-Item -ItemType "directory" -Path $filePath
            }
            # Download artifact from other job
            Start-FileDownload "https://ci.appveyor.com/api/buildjobs/$jobId/artifacts/$($_.fileName)" -FileName $_.fileName
        }
    };
}

Write-Host "Start to build nupkg files"

# This is the CoApp .autopkg file to create.
$autopkgFile = "scripts\coapp\kiwano.autopkg"

# Get the ".autopkg.template" file, replace "@appveyor_version" with the Appveyor version number, then save to the ".autopkg" file.
Get-Content ($autopkgFile + ".template") | ForEach-Object { $_ -replace "@appveyor_version", $env:appveyor_build_version } > $autopkgFile

# Use the CoApp tools to create NuGet native packages from the .autopkg.
Write-NuGetPackage $autopkgFile

# Push all newly created .nupkg files as Appveyor artifacts for later deployment.
Get-ChildItem .\*.nupkg | ForEach-Object { Push-AppveyorArtifact $_.FullName -FileName $_.Name }