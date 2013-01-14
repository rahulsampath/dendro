
/**
  @file seqUtils.txx
  @brief Definitions of the templated functions in the seq module.
  @author Rahul S. Sampath, rahul.sampath@gmail.com
  */

#include <cmath>
#include <vector>
#include <cstdlib>
#include <algorithm>

namespace seq {

  template <typename T>
    bool BinarySearch(const T* arr, unsigned int nelem, const T& key, unsigned int* ret_idx) {
      bool found = false;
      if(nelem > 0) {
        const T* pos = std::lower_bound(arr, (arr + nelem), key);
        if(pos != (arr + nelem)) {
          if((*pos) == key) {
            found = true;
            *ret_idx = pos - arr;
          }
        }
      }
      if(!found) {
        *ret_idx = nelem;
      }
      return found;
    }

  template <typename T>
    int UpperBound(unsigned int nelem, const T* arr, unsigned int startIdx, const T& key) {
      int res = nelem;
      if(startIdx < nelem) {
        const T* pos = std::lower_bound((arr + startIdx), (arr + nelem), key);
        res = pos - arr;
      }
      return res;
    }

  template <typename T>
    bool maxLowerBound(const std::vector<T>& arr, const T& key, unsigned int& ret_idx,
        unsigned int* leftIdx, unsigned int* rightIdx) {
      bool found = false;
      unsigned int stIdx = 0;
      unsigned int endIdx = arr.size();
      if(leftIdx != NULL) {
        stIdx = *leftIdx;
      }
      if(rightIdx != NULL) {
        endIdx = (*rightIdx) + 1;
      }
      if(stIdx < endIdx) {
        if(arr[stIdx] <= key) {
          found = true;
          typename std::vector<T>::const_iterator pos = std::upper_bound((arr.begin() + stIdx), (arr.begin() + endIdx), key);
          if(pos != (arr.begin() + endIdx)) {
            //assert((*pos) > key);
            ret_idx = pos - arr.begin() - 1;
          } else {
            ret_idx = endIdx - 1;
          }
        }
      }
      if(!found) {
        ret_idx = arr.size();
      }
      return found;
    }

  template<typename T>
    void makeVectorUnique(std::vector<T>& vecT, bool isSorted) {
      if(vecT.size() < 2) {
        return;
      }
      if(!isSorted) {
        std::sort( (&(vecT[0])), ( (&(vecT[0])) + (vecT.size()) ) );
      }
      std::vector<T> tmp(vecT.size());
      //Using the array [] is faster than the vector version
      T* tmpPtr = (&(*(tmp.begin())));
      T* vecTptr = (&(*(vecT.begin())));
      tmpPtr[0] = vecTptr[0];
      unsigned int tmpSize = 1;
      unsigned int vecTsz = static_cast<unsigned int>(vecT.size());
      for(unsigned int i = 1; i < vecTsz; ++i) {	
        if(tmpPtr[tmpSize-1] != vecTptr[i]) {
          tmpPtr[tmpSize] = vecTptr[i];
          ++tmpSize;
        }
      }//end for
      tmp.resize(tmpSize);
      swap(vecT, tmp);
    }

  template <typename T>
    void flashsort(T* a, int n, int m, int *ctr)
    {
      const int THRESHOLD = 75;
      const int CLASS_SIZE = 75;     /* minimum value for m */

      /* declare variables */
      int *l, nmin, nmax, i, j, k, nmove, nx, mx;

      T c1,c2,flash,hold;

      /* allocate space for the l vector */
      l=(int*)calloc(m,sizeof(int));

      /***** CLASS FORMATION ****/

      nmin=nmax=0;
      for (i=0 ; i<n ; i++)
        if (a[i] < a[nmin]) nmin = i;
        else if (a[i] > a[nmax]) nmax = i;

      if ( (a[nmax]==a[nmin]) && (ctr==0) )
      {
        printf("All the numbers are identical, the list is sorted\n");
        return;
      }

      c1=(m-1.0)/(a[nmax]-a[nmin]) ;
      c2=a[nmin];

      l[0]=-1; /* since the base of the "a" (data) array is 0 */
      for (k=1; k<m ; k++) l[k]=0;

      for (i=0; i<n ; i++)
      {
        k=floor(c1*(a[i]-c2) );
        l[k]+=1;
      }

      for (k=1; k<m ; k++) l[k]+=l[k-1];

      hold=a[nmax];
      a[nmax]=a[0];
      a[0]=hold; 
      /**** PERMUTATION *****/

      nmove=0;
      j=0;
      k=m-1;

      while(nmove<n)
      {
        while  (j  >  l[k] )
        {
          j++;
          k=floor(c1*(a[j]-c2) ) ;
        }

        flash=a[ j ] ;

        while ( j <= l[k] )
        {
          k=floor(c1*(flash-c2));
          hold=a[ l[k] ];
          a[ l[k] ] = flash;
          l[k]--;
          flash=hold;
          nmove++;
        }
      }

      /**** Choice of RECURSION or STRAIGHT INSERTION *****/

      for (k=0;k<(m-1);k++)
        if ( (nx = l[k+1]-l[k]) > THRESHOLD )  /* then use recursion */
        {
          flashsort(&a[l[k]+1],nx,CLASS_SIZE,ctr);
          (*ctr)++;
        }

        else  /* use insertion sort */
          for (i=l[k+1]-1; i > l[k] ; i--)
            if (a[i] > a[i+1])
            {
              hold=a[i];
              j=i;
              while  (hold  >  a[j+1] )  a[j++]=a[j+1] ;
              a[j]=hold;
            }
      free(l);   /* need to free the memory we grabbed for the l vector */
    }

}//end namespace



