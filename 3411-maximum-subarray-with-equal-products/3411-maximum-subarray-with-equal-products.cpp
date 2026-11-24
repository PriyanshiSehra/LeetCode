class Solution {
public:
    int maxLength(vector<int>& nums) {
        int n = nums.size();
        int ans = 0;

        for (int i = 0; i < n; i++) {
            if (nums[i] == 1) {
                ans = max(ans, 1);
            }

            bool good = true;

            for (int j = i + 1; j < n; j++) {

                // Check if current element is coprime
                // with all previous elements
                for (int k = i; k < j; k++) {
                    if (gcd(nums[k], nums[j]) != 1) {
                        good = false;
                    }
                }

                int len = j - i + 1;

                // Every subarray of length 2 is valid
                if (len == 2 || good) {
                    ans = max(ans, len);
                }
            }
        }

        return ans;
    }
};