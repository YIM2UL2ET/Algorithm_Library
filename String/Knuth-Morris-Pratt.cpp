vector<int> KMP(const string& T, const string& P) {  // 0-index
  int n = T.size(), m = P.size();

  // failure function
  vector<int> f(m);
  for (int k = 0, q = 1; q < m; q++) {
    while (k > 0 && P[k] != P[q]) k = f[k - 1];
    if (P[k] == P[q]) ++k;
    f[q] = k;
  }

  // matching
  vector<int> match;
  for (int k = 0, q = 0; q < n; q++) {
    while (k > 0 && P[k] != T[q]) k = f[k - 1];
    if (P[k] == T[q]) ++k;
    if (k == m) {
      match.push_back(q - m + 1);
      k = f[k];
    }
  }

  return match;
}