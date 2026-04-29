// reference : https://github.com/justiceHui/icpc-teamnote/blob/master/code/Graph/BipartiteMatchingLong.cpp
struct HopcroftKarp {  // 0-index, O(V^2.5)
  int n, m; vector<vector<int>> adj; vector<int> dist, A, B; vector<bool> vst, track;
  HopcroftKarp(int n, int m) : n(n), m(m), adj(n), dist(n), A(n, -1), B(m, -1), vst(n), track(n + m) {}

  void add_edge(int s, int e) { adj[s].push_back(e); }

  int maximum_matching() {
    int res = 0;
    fill(A.begin(), A.end(), -1), fill(B.begin(), B.end(), -1);
    while (bfs()) {
      fill(vst.begin(), vst.end(), 0);
      for (int i = 0; i < n; i++) if (A[i] == -1) res += dfs(i);
    }
    return res;
  }

  vector<pair<int, int>> maximum_matching_edges() {
    int matching = maximum_matching();
    vector<pair<int, int>> edges;
    edges.reserve(matching);
    for (int i = 0; i < n; i++)
      if (A[i] != -1) edges.emplace_back(i, A[i]);
    return edges;
  }

  void dfs_track(int a) {
    if (track[a]) return;
    track[a] = true;
    for (auto &b : adj[a]) {
      track[n + b] = 1;
      dfs_track(B[b]);
    }
  }

  tuple<vector<int>, vector<int>, int> minimum_vertex_cover() {
    int matching = maximum_matching();
    vector<int> lv, rv;
    fill(track.begin(), track.end(), 0);
    for (int i = 0; i < n; i++)
      if (A[i] == -1) dfs_track(i);
    for (int i = 0; i < n; i++)
      if (!track[i]) lv.push_back(i);
    for (int i = 0; i < m; i++)
      if (track[n + i]) rv.push_back(i);
    return {lv, rv, lv.size() + rv.size()};  // sz(lv) + sz(rv) == matching
  }

  tuple<vector<int>, vector<int>, int> maximum_independent_set() {
    auto [a, b, matching] = minimum_vertex_cover();
    vector<int> lv, rv;
    lv.reserve(n - a.size()), rv.reserve(m - b.size());
    for (int i = 0, j = 0; i < n; i++) {
      while (j < a.size() && a[j] < i) j++;
      if (j == a.size() || a[j] != i) lv.push_back(i);
    }
    for (int i = 0, j = 0; i < m; i++) {
      while (j < b.size() && b[j] < i) j++;
      if (j == b.size() || b[j] != i) rv.push_back(i);
    }  // sz(lv) + sz(rv) == n + m - matching
    return {lv, rv, lv.size() + rv.size()};
  }

  vector<vector<int>> minimum_path_cover() {  // n == m
    int matching = maximum_matching();
    vector<vector<int>> res;
    res.reserve(n - matching);
    fill(track.begin(), track.end(), false);
    for (int i = 0; i < n; i++)
      if (!track[n + i] && B[i] == -1) res.push_back(get_path(i));
    return res;  // sz(res) == n - matching
  }

  vector<int> maximum_anti_chain() {  // n == m
    auto [a, b, matching] = minimum_vertex_cover();
    vector<int> res;
    res.reserve(n - a.size() - b.size());
    for (int i = 0, j = 0, k = 0; i < n; i++) {
      while (j < a.size() && a[j] < i) j++;
      while (k < b.size() && b[k] < i) k++;
      if ((j == a.size() || a[j] != i) && (k == b.size() || b[k] != i))
        res.push_back(i);
    }
    return res;  // sz(res) == n - matching
  }

  bool bfs() {
    bool res = false; queue<int> que;
    fill(dist.begin(), dist.end(), 0);
    for (int i = 0; i < n; i++)
      if (A[i] == -1) que.push(i), dist[i] = 1;
    while (!que.empty()) {
      int a = que.front();
      que.pop();
      for (auto b : adj[a]) {
        if (B[b] == -1) res = true;
        else if (!dist[B[b]]) dist[B[b]] = dist[a] + 1, que.push(B[b]);
      }
    }
    return res;
  }

  bool dfs(int a) {
    if (vst[a]) return false;
    vst[a] = true;
    for (auto b : adj[a]) {
      if (B[b] == -1 || dist[B[b]] == dist[a] + 1 && dfs(B[b])) {
        A[a] = b, B[b] = a; return true;
      }
    }
    return false;
  }

  vector<int> get_path(int a) {
    vector<int> path{a};  // B[v] == -1
    while (A[a] != -1) path.push_back(a = A[a]);
    return path;
  };
};