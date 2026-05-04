#!/usr/bin/perl

use warnings;
use strict;
use POSIX;

my @dist;

$dist[128]=0;
$dist[129]=0;
$dist[130]=0;
$dist[131]=0;
$dist[132]=0;
$dist[133]=0;
$dist[134]=0;
$dist[135]=0;
$dist[136]=0;
$dist[137]=0;
$dist[138]=0;
$dist[139]=0;
$dist[140]=0;
$dist[141]=0;
$dist[142]=0;
$dist[143]=0;
$dist[144]=0;
$dist[145]=0;
$dist[146]=0;
$dist[147]=0;
$dist[148]=0;
$dist[149]=0;
$dist[150]=0;
$dist[151]=0;
$dist[152]=0;
$dist[153]=0;
$dist[154]=0;
$dist[155]=0;
$dist[156]=0;
$dist[157]=0;
$dist[158]=0;
$dist[159]=0;
$dist[160]=0;
$dist[161]=0;
$dist[162]=0;
$dist[163]=0;
$dist[164]=0;
$dist[165]=0;
$dist[166]=0;
$dist[167]=0;
$dist[168]=0;
$dist[169]=0;
$dist[170]=0;
$dist[171]=0;
$dist[172]=0;
$dist[173]=0;
$dist[174]=0;
$dist[175]=0;
$dist[176]=0;
$dist[177]=0;
$dist[178]=0;
$dist[179]=0;
$dist[180]=0;
$dist[181]=0;
$dist[182]=0;
$dist[183]=0;
$dist[184]=0;
$dist[185]=0;
$dist[186]=0;
$dist[187]=0;
$dist[188]=0;
$dist[189]=0;
$dist[190]=0;
$dist[191]=0;
$dist[192]=0;
$dist[193]=0;
$dist[194]=0;
$dist[195]=0;
$dist[196]=0;
$dist[197]=0;
$dist[198]=0;
$dist[199]=0;
$dist[200]=0;
$dist[201]=0;
$dist[202]=0;
$dist[203]=0;
$dist[204]=0;
$dist[205]=0;
$dist[206]=0;
$dist[207]=0;
$dist[208]=0;
$dist[209]=0;
$dist[210]=0;
$dist[211]=0;
$dist[212]=1;
$dist[213]=1;
$dist[214]=1;
$dist[215]=1;
$dist[216]=1;
$dist[217]=1;
$dist[218]=1;
$dist[219]=1;
$dist[220]=1;
$dist[221]=2;
$dist[222]=2;
$dist[223]=2;
$dist[224]=2;
$dist[225]=2;
$dist[226]=3;
$dist[227]=3;
$dist[228]=3;
$dist[229]=4;
$dist[230]=4;
$dist[231]=5;
$dist[232]=5;
$dist[233]=6;
$dist[234]=7;
$dist[235]=7;
$dist[236]=8;
$dist[237]=9;
$dist[238]=10;
$dist[239]=11;
$dist[240]=12;
$dist[241]=14;
$dist[242]=15;
$dist[243]=17;
$dist[244]=19;
$dist[245]=21;
$dist[246]=23;
$dist[247]=25;
$dist[248]=27;
$dist[249]=30;
$dist[250]=32;
$dist[251]=35;
$dist[252]=37;
$dist[253]=39;
$dist[254]=41;
$dist[255]=42;
$dist[0]  =46;
$dist[1]  =40;
$dist[2]  =37;
$dist[3]  =34;
$dist[4]  =31;
$dist[5]  =28;
$dist[6]  =26;
$dist[7]  =23;
$dist[8]  =21;
$dist[9]  =19;
$dist[10] =17;
$dist[11] =15;
$dist[12] =14;
$dist[13] =13;
$dist[14] =11;
$dist[15] =10;
$dist[16] =9;
$dist[17] =8;
$dist[18] =8;
$dist[19] =7;
$dist[20] =6;
$dist[21] =6;
$dist[22] =5;
$dist[23] =5;
$dist[24] =4;
$dist[25] =4;
$dist[26] =3;
$dist[27] =3;
$dist[28] =3;
$dist[29] =3;
$dist[30] =2;
$dist[31] =2;
$dist[32] =2;
$dist[33] =2;
$dist[34] =2;
$dist[35] =2;
$dist[36] =1;
$dist[37] =1;
$dist[38] =1;
$dist[39] =1;
$dist[40] =1;
$dist[41] =1;
$dist[42] =1;
$dist[43] =1;
$dist[44] =1;
$dist[45] =1;
$dist[46] =1;
$dist[47] =0;
$dist[48] =0;
$dist[49] =0;
$dist[50] =0;
$dist[51] =0;
$dist[52] =0;
$dist[53] =0;
$dist[54] =0;
$dist[55] =0;
$dist[56] =0;
$dist[57] =0;
$dist[58] =0;
$dist[59] =0;
$dist[60] =0;
$dist[61] =0;
$dist[62] =0;
$dist[63] =0;
$dist[64] =0;
$dist[65] =0;
$dist[66] =0;
$dist[67] =0;
$dist[68] =0;
$dist[69] =0;
$dist[70] =0;
$dist[71] =0;
$dist[72] =0;
$dist[73] =0;
$dist[74] =0;
$dist[75] =0;
$dist[76] =0;
$dist[77] =0;
$dist[78] =0;
$dist[79] =0;
$dist[80] =0;
$dist[81] =0;
$dist[82] =0;
$dist[83] =0;
$dist[84] =0;
$dist[85] =0;
$dist[86] =0;
$dist[87] =0;
$dist[88] =0;
$dist[89] =0;
$dist[90] =0;
$dist[91] =0;
$dist[92] =0;
$dist[93] =0;
$dist[94] =0;
$dist[95] =0;
$dist[96] =0;
$dist[97] =0;
$dist[98] =0;
$dist[99] =0;
$dist[100]=0;
$dist[101]=0;
$dist[102]=0;
$dist[103]=0;
$dist[104]=0;
$dist[105]=0;
$dist[106]=0;
$dist[107]=0;
$dist[108]=0;
$dist[109]=0;
$dist[110]=0;
$dist[111]=0;
$dist[112]=0;
$dist[113]=0;
$dist[114]=0;
$dist[115]=0;
$dist[116]=0;
$dist[117]=0;
$dist[118]=0;
$dist[119]=0;
$dist[120]=0;
$dist[121]=0;
$dist[122]=0;
$dist[123]=0;
$dist[124]=0;
$dist[125]=0;
$dist[126]=0;
$dist[127]=0;

my $total = 0;
for(my $i = 0; $i < 256; $i++)
{
    $total += $dist[$i];
}

my @data_orig;

for(my $i = 0; $i < 1024; $i++)
{
    my $r = floor(rand() * $total);
    if($r >= $total) { $r = $total - 1; }

    my $it = 0;

    while(($it < 256) && ($r > $dist[$it]))
    {
        $r -= $dist[$it];
        $it++;
    }

    if($it == 256) { $it = 0; }
    else           { $dist[$it]--; $total--;}

    push @data_orig, $it;
}

open (my $fh, '>:raw', "weights.mem");

for(my $i = 0; $i < (256 * 1024); $i++)
{
    foreach(@data_orig)
    {
        my $data = $_;
        print $fh map chr, $data;
    }
}
close ($fh);

