# MoaBrowser.ps1 - Invoca al Browser definido en el DDproj.ini
#                 y se queda esperando hasta que finalice el proceso.
# Autor: Bazante German Pablo
# Fecha Creacion: 30/11/2019

# Declaro los parametros de entrada
Param(
    [string] $Browser = "c:\Program Files\Mozilla Firefox\firefox.exe",
    [string] $Parameters = "-no-remote",
    #[string] $Url = "https://www.quora.com/"
    [string] $Url
)
$ScriptVersion =  "1.0.0"

#################################################################################
## Declaracion de Funciones
#################################################################################

#Verifica la existencia del directorio de ejecucion del script
Function Check-Location {
    $Location = Get-Location
    Write-Host "Directorio Actual [$Location]"

    $Location = "c:\MOA\BIN\MoaBrowser"
    $DirExists = Test-Path $Location

    Write-Host "Directorio de Ejecucion [$Location]"

	#Check if Dir exists
	If ($DirExists -eq $true) {
        cd $Location
        Write-Host "[OK]"
    } else {
        Write-Host "[ERROR] No existe el Directorio [$Location]"
        #Abort-Excecution
        Write-Host "Abort-Excecution"
        exit
    }
}


#-------------------------------------------------------------------------------
# Ejecucion del Script
#-------------------------------------------------------------------------------

Write-Host "Version de ServicioMoaPDF : $ScriptVersion"

#Verifico directorio donde se ejecuta el script
Check-Location

Write-Host "Browser [$Browser]"
Write-Host "Parameters [$Parameters]"
Write-Host "Url [$Url]"

Write-Host "Antes"

#Start-Process -FilePath $Browser -ArgumentList '-no-remote https://www.quora.com/' -Wait
Start-Process -FilePath $Browser -ArgumentList "$Parameters $Url" -Wait

Write-Host "Despues"
