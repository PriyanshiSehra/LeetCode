#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int smallestUniqueSubarray(vector<int>& nums) {
        int n = nums.size();
        if (n <= 1) return n;

        // 1. Coordinate compression so values are in [1, n]
        vector<int> vals = nums;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());

        vector<int> s(n + 1);
        for (int i = 0; i < n; ++i) {
            s[i] = lower_bound(vals.begin(), vals.end(), nums[i]) - vals.begin() + 1;
        }
        s[n] = 0; // Sentinel smaller than all elements

        // 2. Build Suffix Array in O(n log n)
        int N = n + 1;
        int alphabet = vals.size() + 1;
        vector<int> p(N), c(N), cnt(max(alphabet, N), 0);

        for (int i = 0; i < N; ++i) cnt[s[i]]++;
        for (int i = 1; i < alphabet; ++i) cnt[i] += cnt[i - 1];
        for (int i = N - 1; i >= 0; --i) p[--cnt[s[i]]] = i;

        c[p[0]] = 0;
        int classes = 1;
        for (int i = 1; i < N; ++i) {
            if (s[p[i]] != s[p[i - 1]]) classes++;
            c[p[i]] = classes - 1;
        }

        vector<int> pn(N), cn(N);
        for (int h = 0; (1 << h) < N; ++h) {
            int shift = 1 << h;
            for (int i = 0; i < N; ++i) {
                pn[i] = p[i] - shift;
                if (pn[i] < 0) pn[i] += N;
            }
            fill(cnt.begin(), cnt.begin() + classes, 0);
            for (int i = 0; i < N; ++i) cnt[c[pn[i]]]++;
            for (int i = 1; i < classes; ++i) cnt[i] += cnt[i - 1];
            for (int i = N - 1; i >= 0; --i) p[--cnt[c[pn[i]]]] = pn[i];

            cn[p[0]] = 0;
            classes = 1;
            for (int i = 1; i < N; ++i) {
                pair<int, int> cur = {c[p[i]], c[(p[i] + shift) % N]};
                pair<int, int> prev = {c[p[i - 1]], c[(p[i - 1] + shift) % N]};
                if (cur != prev) classes++;
                cn[p[i]] = classes - 1;
            }
            c.swap(cn);
            if (classes == N) break;
        }

        // Remove the sentinel suffix at p[0]
        vector<int> sa(p.begin() + 1, p.end());

        // 3. Build LCP array in O(n) using Kasai's algorithm
        vector<int> rank(n);
        for (int i = 0; i < n; ++i) rank[sa[i]] = i;

        vector<int> lcp(n - 1, 0);
        int k = 0;
        for (int i = 0; i < n; ++i) {
            if (rank[i] == n - 1) {
                k = 0;
                continue;
            }
            int j = sa[rank[i] + 1];
            while (i + k < n && j + k < n && s[i + k] == s[j + k]) {
                k++;
            }
            lcp[rank[i]] = k;
            if (k > 0) k--;
        }

        // 4. Find the minimum unique subarray length
        int ans = n;
        for (int i = 0; i < n; ++i) {
            int max_lcp = 0;
            if (i > 0) max_lcp = max(max_lcp, lcp[i - 1]);
            if (i < n - 1) max_lcp = max(max_lcp, lcp[i]);

            int suffix_len = n - sa[i];
            if (max_lcp + 1 <= suffix_len) {
                ans = min(ans, max_lcp + 1);
            }
        }

        return ans;
    }
};