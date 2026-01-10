Param(
    [string]$Type = 'patch'  # patch | minor | major
)

$path = Join-Path $PSScriptRoot '..\version.txt' | Resolve-Path -Relative
$v = (Get-Content $path).Trim()
if (-not $v) { Write-Error "version.txt is empty"; exit 1 }
$parts = $v.Split('.') | ForEach-Object { [int]$_ }
switch ($Type) {
    'patch' { $parts[2] += 1 }
    'minor' { $parts[1] += 1; $parts[2] = 0 }
    'major' { $parts[0] += 1; $parts[1] = 0; $parts[2] = 0 }
    default { Write-Error "Unknown type: $Type"; exit 1 }
}
$new = ($parts -join '.')
Set-Content -Path $path -Value $new
Write-Output "Bumped version to $new"
# optionally commit & tag (commented out):
# git add $path; git commit -m "Bump version to $new"; git tag v$new; git push && git push --tags

