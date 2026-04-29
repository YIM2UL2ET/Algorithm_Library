typedef complex<double> cpx;
void fft(vector<cpx> &A, bool inv) {  // Cooley-Tukey Algorithm, A = 2^len, O(nlogn)
  int n = A.size(), len = 31 - __builtin_clz(n);
  
  vector<int> rev(n);
  for (int i = 0; i < n; i++) rev[i] = (rev[i / 2] | (i & 1) << len) / 2;
  for (int i = 0; i < n; i++)
    if (i < rev[i]) swap(A[i], A[rev[i]]);

  for (int k = 1; k < n; k <<= 1) {
    double ang = (inv ? M_PI / k : -M_PI / k);
    cpx wlen(cos(ang), sin(ang));
    for (int i = 0; i < n; i += 2 * k) {
      cpx w(1);
      for (int j = 0; j < k; j++) {
        cpx even = A[i + j], odd = A[i + j + k] * w;
        A[i + j] = even + odd;
        A[i + j + k] = even - odd;
        w *= wlen;
      }
    }
  }
  if (inv) for (int i = 0; i < n; i++) A[i] /= n;
}

vector<cpx> multiply(vector<cpx> &A, vector<cpx> &B) {
  int n = 1 << (32 - __builtin_clz(A.size() + B.size() - 1));
  A.resize(n), B.resize(n);
  fft(A, false), fft(B, false);
  vector<cpx> res(n);
  for (int i = 0; i < n; i++) res[i] = A[i] * B[i];
  fft(res, true);
  return res;
}