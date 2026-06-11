$ErrorActionPreference = "Stop"

# Clean
Remove-Item -Recurse -Force EU04_WSWP,EU04_WSWP.zip -ErrorAction SilentlyContinue

# Make dir
mkdir EU04_WSWP

# Build
msbuild "Wasp Engine.sln" /p:Configuration=Release /p:Platform=x64

# Copy everything into dir
cp ".\x64\Release\Wasp Engine.exe" EU04_WSWP\EU04_WSWP.exe
cp -r res EU04_WSWP\
cp -r packaging\* EU04_WSWP\

# Zip it up
Compress-Archive -Path EU04_WSWP -DestinationPath EU04_WSWP.zip