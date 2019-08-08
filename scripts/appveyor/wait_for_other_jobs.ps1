. .\scripts\appveyor\appveyor_get_build.ps1

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

# Set flag to deploy
$env:flag_to_deploy = $success