# DEPRECATION NOTICE

Please see [Metallicizer](https://nthn.gumroad.com/l/jk_nthn_metallicizer), my M4L Plugin with [Nathan Blair](https://github.com/ncblair). That is the successor to the work I was doing in here, but I'll leave this code open for anyone who's curious.

## externs in this repo
 - vocalc
    - a small and simple band calculator for vocoders. 
    - more or less just a C implementation of the calculations laid out [here](https://cycling74.com/tutorials/a-basic-vocoder-tutorial-part-1).
 - jkVocoder
    - a custom vocoder also heavily based on the above tutorial. I've given it a crude ability to adjust its number of bands, and also allowed the bandwidth to dip much further down than 10%. At less than 3% bandwidth, the plugin totally changes character and starts to sound like a physically modeled resonator.
