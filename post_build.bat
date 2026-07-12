echo ### Post build event START ###
echo Removing data.pak if exists...
IF EXIST data.pak DEL /F data.pak
echo ...done.
echo Compressing new data.pak file...
compressor.exe data.pak + assets
echo ...done.
echo ### Post build event END ###