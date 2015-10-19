// http://www.jonolick.com/code.html

#pragma once

void jo_write_tga(const char *filename, void *rgba, int width, int height, int numChannels) {
    FILE *fp = fopen(filename, "wb");
    if(!fp) {
        return;   
    }
    // Header
    fwrite("\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00", 12, 1, fp);
    fwrite(&width, 2, 1, fp);
    fwrite(&height, 2, 1, fp);
    int bpc = numChannels * 8; // 8 bits per channel
    fwrite(&bpc, 2, 1, fp);
    // Swap RGBA to BGRA if using 3 or more channels
    int remap[4] = {numChannels >= 3 ? 2 : 0, 1, numChannels >= 3 ? 0 : 2, 3};
    char *s = (char *)rgba;
    for(int y = height-1; y >= 0; --y)  {
        int i = (y * width) * numChannels;
        for(int x = i; x < i+width*numChannels; x += numChannels) {
            for(int j = 0; j < numChannels; ++j) {
                fputc(s[x+remap[j]], fp);
            }
        }
    }
    fclose(fp);
}


bool jo_write_wav(const char *filename, short numChannels, short bitsPerSample, int sampleRateHz, const void *data, 
int size) {
    FILE *fp = fopen(filename, "wb");
    if(!fp) {
        return false;
    }
    fwrite("RIFF", 1, 4, fp);
    int length = size + 44 - 8;
    fwrite(&length, 1, 4, fp);
    fwrite("WAVEfmt \x10\x00\x00\x00\x01\x00", 1, 14, fp);
    fwrite(&numChannels, 1, 2, fp);
    fwrite(&sampleRateHz, 1, 4, fp);
    int bpsec = numChannels * sampleRateHz * bitsPerSample/8;
    fwrite(&bpsec, 1, 4, fp);
    short bpsamp = numChannels * bitsPerSample/8;
    fwrite(&bpsamp, 1, 2, fp);
    fwrite(&bitsPerSample, 1, 2, fp);
    fwrite("data", 1, 4, fp);
    fwrite(&size, 1, 4, fp);
    fwrite(data, 1, size, fp);
    fclose(fp);
    return true;
}

// Bi-linear Texture Sampling
static inline unsigned char Sample1(unsigned char *d, int width, int height, float u, float v) {
    int ww = width-1, hh = height-1;
    float fu = (u + 1000.0f) * ww; // NOTE: assumes less than 1000 repeats
    float fv = (v + 1000.0f) * hh;
    int u1 = ((int)fu) % ww;
    int v1 = ((int)fv) % hh;
    int u2 = (u1 + 1) % ww;
    int v2 = (v1 + 1) % hh;
    float fracu = fu - floorf( fu );
    float fracv = fv - floorf( fv );
    float w[4] = {(1 - fracu) * (1 - fracv), fracu * (1 - fracv), (1 - fracu) * fracv, fracu *  fracv};
    unsigned char c[4] = { d[v1*width + u1], d[v1*width + u2], d[v2*width + u1], d[v2*width + u2] };
    return (unsigned char)(c[0]*w[0] + c[1]*w[1] + c[2]*w[2] + c[3]*w[3]);
}