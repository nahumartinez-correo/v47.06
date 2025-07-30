\moa\bin\ddinit -nibamindd 
\moa\bin\import apl -v -nibamindd <src\appls
\moa\bin\import icn -v -nibamindd -fsrc\icons <src\icn
\moa\bin\import dsc -v -nibamindd <src\common\dsc
\moa\bin\import fld -v -nibamindd <src\common\fld
\moa\bin\import csr -v -nibamindd <src\common\csr
\moa\bin\import pat -v -nibamindd <src\common\pat

\moa\bin\bc -nibamindd -v src\common\lst
\moa\bin\bc -nibamindd -v -I"src\include" src\common\ofb.ofb
