[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$LevelName,

    [Parameter(Mandatory = $true)]
    [string]$MapImagePath,

    [Parameter(Mandatory = $true)]
    [string]$OutputPath,

    [int]$TileSize = 16,
    [int]$RoomTileWidth = 16,
    [int]$RoomTileHeight = 16,
    [string]$WalkableColor = "235,137,49"
)

Add-Type -AssemblyName System.Drawing

$walkableRgb = $WalkableColor.Split(",") | ForEach-Object { [int]$_.Trim() }
if ($walkableRgb.Count -ne 3) {
    throw "WalkableColor must be in the form R,G,B."
}

function Test-WalkableTile {
    param(
        [System.Drawing.Color]$Color,
        [int[]]$ExpectedRgb
    )

    return $Color.R -eq $ExpectedRgb[0] -and
           $Color.G -eq $ExpectedRgb[1] -and
           $Color.B -eq $ExpectedRgb[2]
}

$resolvedImagePath = (Resolve-Path $MapImagePath).Path
$bitmap = [System.Drawing.Bitmap]::new($resolvedImagePath)

try {
    $tilesWide = [math]::Floor($bitmap.Width / $TileSize)
    $tilesHigh = [math]::Floor($bitmap.Height / $TileSize)
    $roomsWide = [math]::Ceiling($tilesWide / $RoomTileWidth)
    $roomsHigh = [math]::Ceiling($tilesHigh / $RoomTileHeight)

    $rooms = New-Object System.Collections.Generic.List[object]

    for ($roomY = 0; $roomY -lt $roomsHigh; $roomY++) {
        for ($roomX = 0; $roomX -lt $roomsWide; $roomX++) {
            $rows = New-Object System.Collections.Generic.List[string]
            $openTiles = 0

            for ($localY = 0; $localY -lt $RoomTileHeight; $localY++) {
                $tileY = $roomY * $RoomTileHeight + $localY
                $rowBuilder = New-Object System.Text.StringBuilder

                for ($localX = 0; $localX -lt $RoomTileWidth; $localX++) {
                    $tileX = $roomX * $RoomTileWidth + $localX

                    if ($tileX -ge $tilesWide -or $tileY -ge $tilesHigh) {
                        [void]$rowBuilder.Append("#")
                        continue
                    }

                    $sampleX = [math]::Min(($tileX * $TileSize) + [math]::Floor($TileSize / 2), $bitmap.Width - 1)
                    $sampleY = [math]::Min(($tileY * $TileSize) + [math]::Floor($TileSize / 2), $bitmap.Height - 1)
                    $pixelColor = $bitmap.GetPixel($sampleX, $sampleY)

                    if (Test-WalkableTile -Color $pixelColor -ExpectedRgb $walkableRgb) {
                        [void]$rowBuilder.Append(".")
                        $openTiles++
                    }
                    else {
                        [void]$rowBuilder.Append("#")
                    }
                }

                $rows.Add($rowBuilder.ToString())
            }

            if ($openTiles -gt 0) {
                $rooms.Add([PSCustomObject]@{
                    GridX = $roomX
                    GridY = $roomY
                    OpenTiles = $openTiles
                    Rows = $rows
                })
            }
        }
    }

    $activeRoomLookup = @{}
    foreach ($room in $rooms) {
        $activeRoomLookup["$($room.GridX),$($room.GridY)"] = $true
    }

    $lines = New-Object System.Collections.Generic.List[string]
    $lines.Add("LEVEL $LevelName")
    $lines.Add("SOURCE_MAP $MapImagePath")
    $lines.Add("TILE_SIZE $TileSize")
    $lines.Add("ROOM_TILE_SIZE $RoomTileWidth $RoomTileHeight")
    $lines.Add("GRID_SIZE $roomsWide $roomsHigh")
    $lines.Add("ROOM_COUNT $($rooms.Count)")
    $lines.Add("SYMBOLS . WALKABLE")
    $lines.Add("SYMBOLS # BLOCKED")
    $lines.Add("NEIGHBOR_ORDER N E S W")

    foreach ($room in $rooms) {
        $north = if ($activeRoomLookup.ContainsKey("$($room.GridX),$($room.GridY - 1)")) { "$($room.GridX),$($room.GridY - 1)" } else { "-" }
        $east = if ($activeRoomLookup.ContainsKey("$($room.GridX + 1),$($room.GridY)")) { "$($room.GridX + 1),$($room.GridY)" } else { "-" }
        $south = if ($activeRoomLookup.ContainsKey("$($room.GridX),$($room.GridY + 1)")) { "$($room.GridX),$($room.GridY + 1)" } else { "-" }
        $west = if ($activeRoomLookup.ContainsKey("$($room.GridX - 1),$($room.GridY)")) { "$($room.GridX - 1),$($room.GridY)" } else { "-" }

        $lines.Add("")
        $lines.Add("ROOM $($room.GridX) $($room.GridY)")
        $lines.Add("OPEN_TILES $($room.OpenTiles)")
        $lines.Add("NEIGHBORS $north $east $south $west")
        $lines.Add("MAP")
        foreach ($row in $room.Rows) {
            $lines.Add($row)
        }
        $lines.Add("END")
    }

    $outputDirectory = Split-Path -Parent $OutputPath
    if ($outputDirectory) {
        New-Item -ItemType Directory -Path $outputDirectory -Force | Out-Null
    }

    Set-Content -Path $OutputPath -Value $lines -Encoding ascii
    Write-Output "Wrote $($rooms.Count) rooms to $OutputPath"
}
finally {
    $bitmap.Dispose()
}
