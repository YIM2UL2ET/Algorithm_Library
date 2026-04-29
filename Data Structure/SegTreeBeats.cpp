struct SegBeats {
  struct Node {
    ll maxv = 0, maxc = 0, smax = 0, psum = 0;
  };

  Node tree[4040404];

  Node merge(const Node& n1, const Node& n2) {
    if (n1.maxv > n2.maxv)
      return {n1.maxv, n1.maxc, max(n1.smax, n2.maxv), n1.psum + n2.psum};
    if (n1.maxv < n2.maxv)
      return {n2.maxv, n2.maxc, max(n2.smax, n1.maxv), n1.psum + n2.psum};
    return {n1.maxv, n1.maxc + n2.maxc, max(n1.smax, n2.smax),
            n1.psum + n2.psum};
  }

  void build(int n, int s, int e, ll arr[]) {
    if (s == e) {
      tree[n] = {arr[s], 1, -1, arr[s]};
    } else {
      build(n << 1, s, (s + e >> 1), arr);
      build(n << 1 | 1, (s + e >> 1) + 1, e, arr);
      tree[n] = merge(tree[n << 1], tree[n << 1 | 1]);
    }
  }

  // prop 판단 기준 -> 하위노드의 maxv가 상위노드의 maxv보다 크면 prop
  // psum과 maxv를 업데이트해야함. update의 prop조건에 따라서 smax는 모든 갱신
  // 값보다 작음 (smax < x)
  void propagate(int n, int s, int e) {
    if (s == e) return;
    int l = n << 1, r = n << 1 | 1;
    if (tree[l].maxv > tree[n].maxv) {
      tree[l].psum -= (tree[l].maxv - tree[n].maxv) * tree[l].maxc;
      tree[l].maxv = tree[n].maxv;
    }
    if (tree[r].maxv > tree[n].maxv) {
      tree[r].psum -= (tree[r].maxv - tree[n].maxv) * tree[r].maxc;
      tree[r].maxv = tree[n].maxv;
    }
  }

  // maxv보다 x가 크거나 같음 -> 중단 (min값으로 갱신하는건데, 최댓값이 x보다
  // 크면 의미가 없음) smax보다 x가 클 경우 -> 갱신 (smax보다 큰 값들은 maxv밖에
  // 없으므로 maxv와 psum을 갱신하고 prop해준다) smax보다 x가 작거나 같을 경우
  // -> 하위 노드 update 후 merge
  void update(int n, int s, int e, int l, int r, ll x) {
    propagate(n, s, e);
    if (r < s || e < l || tree[n].maxv <= x) return;

    if (l <= s && e <= r && tree[n].smax < x) {
      tree[n].psum -= (tree[n].maxv - x) * tree[n].maxc;
      tree[n].maxv = x;
      propagate(n, s, e);
    } else {
      update(n << 1, s, (s + e >> 1), l, r, x);
      update(n << 1 | 1, (s + e >> 1) + 1, e, l, r, x);
      tree[n] = merge(tree[n << 1], tree[n << 1 | 1]);
    }
  }

  ll query1(int n, int s, int e, int l, int r) {  // max query
    propagate(n, s, e);
    if (r < s || e < l) return 0;
    if (l <= s && e <= r) return tree[n].maxv;
    return max(query1(n << 1, s, (s + e >> 1), l, r),
               query1(n << 1 | 1, (s + e >> 1) + 1, e, l, r));
  }

  ll query2(int n, int s, int e, int l, int r) {  // sum query
    propagate(n, s, e);
    if (r < s || e < l) return 0;
    if (l <= s && e <= r) return tree[n].psum;
    return query2(n << 1, s, (s + e >> 1), l, r) +
           query2(n << 1 | 1, (s + e >> 1) + 1, e, l, r);
  }
} seg;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr), cout.tie(nullptr);

  ll n, m, arr[1010101];
  cin >> n;
  for (int i = 1; i <= n; i++) cin >> arr[i];
  seg.build(1, 1, n, arr);
  cin >> m;
  for (int i = 0; i < m; i++) {
    ll cmd, l, r, x;
    cin >> cmd >> l >> r;
    if (cmd == 1) {
      cin >> x;
      seg.update(1, 1, n, l, r, x);
    } else if (cmd == 2) {
      cout << seg.query1(1, 1, n, l, r) << '\n';
    } else {
      cout << seg.query2(1, 1, n, l, r) << '\n';
    }
  }

  return 0;
}