# LaTeX2HTML 2008 (1.71)
# Associate images original text with physical files.


$key = q/10^{-6};MSF=1.6;LFS=11;AAT/;
$cached_env_img{$key} = q|<IMG
 WIDTH="40" HEIGHT="21" ALIGN="BOTTOM" BORDER="0"
 SRC="|."$dir".q|img1.png"
 ALT="$10^{-6}$">|; 

$key = q/>;MSF=1.6;LFS=11;AAT/;
$cached_env_img{$key} = q|<IMG
 WIDTH="18" HEIGHT="31" ALIGN="MIDDLE" BORDER="0"
 SRC="|."$dir".q|img4.png"
 ALT="$&gt;$">|; 

$key = q/<;MSF=1.6;LFS=11;AAT/;
$cached_env_img{$key} = q|<IMG
 WIDTH="18" HEIGHT="31" ALIGN="MIDDLE" BORDER="0"
 SRC="|."$dir".q|img3.png"
 ALT="$&lt;$">|; 

$key = q/leq;MSF=1.6;LFS=11;AAT/;
$cached_env_img{$key} = q|<IMG
 WIDTH="18" HEIGHT="31" ALIGN="MIDDLE" BORDER="0"
 SRC="|."$dir".q|img5.png"
 ALT="$\leq$">|; 

$key = q/geq;MSF=1.6;LFS=11;AAT/;
$cached_env_img{$key} = q|<IMG
 WIDTH="18" HEIGHT="31" ALIGN="MIDDLE" BORDER="0"
 SRC="|."$dir".q|img6.png"
 ALT="$\geq$">|; 

$key = q/^2;MSF=1.6;LFS=11;AAT/;
$cached_env_img{$key} = q|<IMG
 WIDTH="12" HEIGHT="21" ALIGN="BOTTOM" BORDER="0"
 SRC="|."$dir".q|img2.png"
 ALT="$^2$">|; 

1;

