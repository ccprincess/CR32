//#ifndef HELPER_H
//#define HELPER_H


///*
// * 16进制数转换成QString显示
// */
//QString MyhexToString(unsigned char *in, int len)
//{
//    int i;
//    unsigned char inChar,hi,lo;
//    QString s;

//    for (i=0; i<len; i++)
//    {
//        inChar = in[i];

//        hi = (inChar & 0xF0) >> 4;
//        if (hi > 9)
//            hi = 'A' + (hi - 0x0A);
//        else
//            hi += 0x30;
//        s.append(hi);

//        lo = inChar & 0x0F;
//        if (lo > 9)
//            lo = 'A' + (lo - 0x0A);
//        else
//            lo += 0x30;
//        s.append(lo);

//        s.append(0x20);
//    }

//    return s;
//}


//#endif // HELPER_H
