#include "mex.h" 
#include <math.h>
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, 
        const mxArray *prhs[])
{
    int i, j, counter, Nbatch, Nmax, ind, L, lx, dx;
    int ix, iy, iz, imap, xr, yr, zr, iW, iW2;
    double *Params, *H,*dW, *yres, *yrec;
    double tErr;
        
    Nbatch  = mxGetN(prhs[1]);
    /* Nbatch is the number of examples, N is the number of filters */
    
    yres      = mxGetPr(prhs[0]); /* N by Nbatch */
    H         = mxGetPr(prhs[1]); /* N by N */    
    Params         = mxGetPr(prhs[2]); /* N by N */    
    Nmax      = (int) Params[0];
    tErr      = Params[1];        
    L         = (int) Params[3];
    lx        = (int) Params[4];
    dx        = lx/2;
    imap        = (int) Params[6];
    dW         = mxGetPr(prhs[3]); /* N by N */    
    
    counter = 0;
    for (i=0; i<Nbatch; i++)
        for (j=0;j<Nmax;j++){
            ind = (int) H[Nmax*i+j];
            if (ind<0)
                break;

            iz = floor(ind/ (L*L)); ind = ind - L*L * iz; iy = floor(ind/L);
            ix = ind - L * iy;
            if ((iz+1)==imap){
                for (yr=max(0, iy-dx); yr<min(iy+dx+1, L); yr++)
                    for (xr=max(0, ix-dx); xr<min(ix+dx+1, L); xr++)
                    {
                    iW  = counter*lx*lx +(yr+dx-iy)*lx + xr+dx-ix;
                    iW2 = iz*lx*lx +(yr+dx-iy)*lx + xr+dx-ix;

                    yres[L*L*i + yr*L + xr] -= dW[iW];
                    }
                counter++;
            }
        }
    
}


