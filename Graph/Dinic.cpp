const int SZ = 101010, INF = 1e9;
template <typename Capacity, typename Weight>
struct Dinic {  // 0-index, O(V^2E)
  struct Edge {
    int vtx; Capacity cap; Weight dst; int rvs;
    Edge(int vtx, Capacity cap, Weight dst, int rvs) : vtx(vtx), cap(cap), dst(dst), rvs(rvs) {}
  };

  Weight dist[SZ]; int n, s, t, cnt[SZ]; bool inQ[SZ], vst[SZ]; vector <Edge> adj[SZ];
  Dinic(int n, int s, int t) : n(n), s(s), t(t) {}

  void add_edge(int u, int v, Capacity capUV, Weight dstUV, Capacity capVU, Weight dstVU) {
    adj[u].emplace_back(v, capUV, dstUV, (int)adj[v].size());
    adj[v].emplace_back(u, capVU, dstVU, (int)adj[u].size() - 1);
  }

  pair<Capacity, Weight> mcmf() {
    Weight mc = 0; Capacity mf = 0;
    while (spfa()) {
      Capacity res = 0;
      for (int i = 0; i < n; i++) cnt[i] = vst[i] = 0;
      while (res = dfs(s, INF)) {
        mc += dist[t] * res, mf += res;
        for (int i = 0; i < n; i++) vst[i] = 0;
      }
    }
    return make_pair(mc, mf);
  }

  bool spfa() {
    queue<int> Q;
    for (int i = 0; i < n; i++) dist[i] = INF, inQ[i] = false;
    Q.push(s), inQ[s] = true, dist[s] = 0;
    while (!Q.empty()) {
      int cur = Q.front(); Q.pop(), inQ[cur] = false;
      for (const auto& [nxt, cap, dst, rvs] : adj[cur]) {
        if (dist[nxt] > dist[cur] + dst && cap > 0) {
          dist[nxt] = dist[cur] + dst;
          if (!inQ[nxt]) Q.push(nxt), inQ[nxt] = true;
        }
      }
    }
    return dist[t] < INF;
  }

  Capacity dfs(int cur, Capacity flow) {
    vst[cur] = true; if (cur == t) return flow;
    while (cnt[cur] < (int)adj[cur].size()) {
      auto& [nxt, cap, dst, rvs] = adj[cur][cnt[cur]];
      if (!vst[nxt] && dist[nxt] == dist[cur] + dst && cap > 0) {
        Capacity res = dfs(nxt, min(flow, cap));
        if (res > 0) {
          cap -= res, adj[nxt][rvs].cap += res;
          return res;
        }
      }
      cnt[cur]++;
    }
    return 0;
  }
};