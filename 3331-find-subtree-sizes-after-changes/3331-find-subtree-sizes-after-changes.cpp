class Solution {
public:
    void findParent(int node, vector<vector<int>>& tree, string& s,
                    vector<int>& last, vector<int>& newParent) {

        int ch = s[node] - 'a';

        int old = last[ch];

        if (old != -1) {
            newParent[node] = old;
        }

        last[ch] = node;

        for (int child : tree[node]) {
            findParent(child, tree, s, last, newParent);
        }

        last[ch] = old;
    }

    int dfs(int node, vector<vector<int>>& tree, vector<int>& ans) {
        int size = 1;

        for (int child : tree[node]) {
            size += dfs(child, tree, ans);
        }

        ans[node] = size;
        return size;
    }

    vector<int> findSubtreeSizes(vector<int>& parent, string s) {
        int n = parent.size();

        vector<vector<int>> tree(n);

        // Build original tree
        for (int i = 1; i < n; i++) {
            tree[parent[i]].push_back(i);
        }

        vector<int> newParent = parent;
        vector<int> last(26, -1);

        // Find new parents
        findParent(0, tree, s, last, newParent);

        // Build modified tree
        vector<vector<int>> newTree(n);

        for (int i = 1; i < n; i++) {
            newTree[newParent[i]].push_back(i);
        }

        // Calculate subtree sizes
        vector<int> ans(n, 0);

        dfs(0, newTree, ans);

        return ans;
    }
};