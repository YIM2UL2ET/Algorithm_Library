const int SZ = 202020;
int sa[SZ], lcp[SZ], ra[SZ];

void suffix_array(const string& T) {  // O(nlog^2n)
  int n = T.size(), tmp[SZ];
  for (int i = 0; i < n; i++) sa[i] = i, ra[i] = T[i];

  for (int len = 1; len < n; len <<= 1) {
    auto cmp = [&](int a, int b) -> bool {
      if (ra[a] != ra[b]) return ra[a] < ra[b];
      int na = (a + len >= n ? -1 : ra[a + len]);
      int nb = (b + len >= n ? -1 : ra[b + len]);
      return na < nb;
    };
    sort(sa, sa + n, cmp);

    tmp[sa[0]] = 0;
    for (int i = 1; i < n; i++) {
      tmp[sa[i]] = tmp[sa[i - 1]];
      if (cmp(sa[i], sa[i - 1]) || cmp(sa[i - 1], sa[i])) tmp[sa[i]]++;
    }
    copy(tmp, tmp + n, ra);
  }
}

void lcp_array(const string& T) {  // not suffix array calc -> O(n)
  int n = T.size();
  suffix_array(T);
  for (int i = 0; i < n; i++) ra[sa[i]] = i;
  for (int i = 0, k = 0; i < n; i++, k = max(0, k - 1)) {
    if (ra[i] == n - 1) continue;
    for (int j = sa[ra[i] + 1]; T[i + k] == T[j + k]; k++);
    lcp[ra[i]] = k;
  }
}