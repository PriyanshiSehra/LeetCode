class Solution {
public:
    int minimumChanges(string s, int k) {
        int n = s.size();

        vector<vector<int>> cost(n, vector<int>(n, 0));

        // Precompute cost to make every substring semi-palindrome
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                int len = j - i + 1;
                cost[i][j] = len;

                // Try every proper divisor
                for (int d = 1; d < len; d++) {
                    if (len % d != 0) continue;

                    int changes = 0;

                    // Create groups based on positions modulo d
                    for (int r = 0; r < d; r++) {
                        int cnt = len / d;

                        for (int t = 0; t < cnt / 2; t++) {
                            int left = i + r + t * d;
                            int right = i + r + (cnt - 1 - t) * d;

                            if (s[left] != s[right]) {
                                changes++;
                            }
                        }
                    }

                    cost[i][j] = min(cost[i][j], changes);
                }
            }
        }

        // DP: dp[p][i] = minimum changes to partition
        // first i characters into p substrings

        const int INF = 1e9;

        vector<vector<int>> dp(k + 1, vector<int>(n + 1, INF));

        dp[0][0] = 0;

        for (int p = 1; p <= k; p++) {
            for (int i = 2 * p; i <= n; i++) {
                for (int j = 2 * (p - 1); j <= i - 2; j++) {
                    dp[p][i] = min(
                        dp[p][i],
                        dp[p - 1][j] + cost[j][i - 1]
                    );
                }
            }
        }

        return dp[k][n];
    }
};