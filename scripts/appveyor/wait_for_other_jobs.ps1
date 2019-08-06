. .\scripts\appveyor\appveyor_get_build.ps1
. .\scripts\appveyor\appveyor_get_artifacts.ps1

# Only deploy when commit message contains "[deploy]"
if (!(Get-AppVeyorBuild).build.message.Contains('[deploy]')) { return }

# Deploy once on last job
if ($env:APPVEYOR_JOB_NUMBER -ne $env:job_to_deploy) { return }

# Wait for other jobs complete
Write-Host "Waiting for other jobs to complete"

[datetime]$stop = ([datetime]::Now).AddMinutes($env:time_out_mins)
[bool]$success = $false

while(!$success -and ([datetime]::Now) -lt $stop) {
    $success = $true
    (Get-AppVeyorBuild).build.jobs | foreach-object {
        if (($_.jobId -ne $env:APPVEYOR_JOB_ID) -and ($_.status -ne "success")) {
            $success = $false
        }
        Write-Host "Job `"$($_.jobId)`" $($_.status)";
    }
    if (!$success) { Start-sleep 5 }
}

if (!$success) {
    throw "Test jobs were not finished in $env:time_out_mins minutes"
}

# get job artifacts
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

# Set flag to deploy
$env:flag_to_deploy = $success