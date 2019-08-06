function Get-AppVeyorArtifacts {
    param(
        [string]$Job
    )

    if (-not ($env:APPVEYOR_API_TOKEN)) {
        throw "missing api token for AppVeyor."
    }
    if (-not ($env:APPVEYOR_ACCOUNT_NAME)) {
        throw "not an appveyor build."
    }

    return Invoke-RestMethod -Method GET -Uri "https://ci.appveyor.com/api/buildjobs/$Job/artifacts" -Headers @{
        "Authorization" = "Bearer $env:APPVEYOR_API_TOKEN"
        "Content-type"  = "application/json"
    }
}