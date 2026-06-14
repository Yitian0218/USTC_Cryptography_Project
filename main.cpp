/*
 * 本代码使用了C++和Python
 * 其中C++仅使用原生库，而Python使用了pandas和matplotlib
 * 此外，plot_frequency中要求本机存在原生中文字体，不然无法正常输出
 *
 * 在本实验的测评工具中，默认alpha=0.05
 */

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <cmath>
#include <random>
#include <filesystem>

namespace fs = std::filesystem;

std::vector<int> get_rand_num_by_rand(int rand_num_max, int rand_num_count);

std::vector<double> get_frequency(const std::vector<int>& rand_num_list, int rand_num_max);

void save_frequency(const std::vector<double>& rand_num_frequency, const std::string& filename);

std::map<int,int> get_gap(const std::vector<int>& rand_num_list, int rand_num_max);

void save_gap(const std::map<int,int>& rand_num_gap, const std::string& filename);

bool Chi2_test(const std::vector<double>& rand_num_frequency, int rand_num_count);

double get_Chi2(unsigned int df);

bool ks_test(const std::vector<double>& rand_num_frequency, int rand_num_count);

bool autocorrelation_test(const std::vector<int>& rand_num_list, int max_lag);

bool runs_test(const std::vector<int>& rand_num_list);

bool gap_test(std::map<int,int>& rand_num_gap, int rand_num_max);

std::vector<int> get_rand_num_by_mt19937(int rand_num_max, int rand_num_count);

std::vector<double> get_rand_num_by_Box_Muller(double mean, double variance, int rand_num_count);

void save_normal(const std::vector<double>& rand_num_normal, double mean, double variance, const std::string& filename);

double normal_inv(double p, double mean, double variance);

bool normal_test(const std::vector<double>& rand_num_normal, double mean, double variance);

void nist_test(const std::vector<int>& rand_num_list);

int main() {

    // 设置随机数上限与随机数生成个数，随机数数组，随机数频率数组与随机数间隔
    constexpr int rand_num_max = 100, rand_num_count = 10000 * rand_num_max;
    std::vector<int> rand_num_list;
    std::vector<double> rand_num_frequency;
    std::map<int,int> rand_num_gap ;
    bool rand_n = true, mt19937 = true, normal = true;

    // 创建输出文件夹
    fs::create_directories("output");

    // 使用rand()%N获取[0,N-1]上均匀分布的随机数

    std::cout << "================================" << std::endl;
    std::cout << "Start Generating Random Numbers by Rand()%N" << std::endl;

    // 获取以rand()%N方式生成的随机数数组
    rand_num_list = get_rand_num_by_rand(rand_num_max, rand_num_count);

    std::cout << "Successfully Generated Random Numbers by Rand()%N" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "Start Calculate the Frequency And the Gap of Random Numbers" << std::endl;

    // 统计[0,N-1]中每一个数在随机数数组中的出现频率
    rand_num_frequency = get_frequency(rand_num_list, rand_num_max);

    // 输出结果为csv并利用python脚本绘制折线图
    save_frequency(rand_num_frequency, "output/Rand()%N_frequency");

    // 统计[0,N-1]中每一个数在随机数数组中重复的间隔，其中rand_num_gap的第index项为index数的不同距离的map，键为距离，值为出现频率，其中距离为两个相同数的index相减
    rand_num_gap = get_gap(rand_num_list, rand_num_max);

    // 输出结果为csv并利用python脚本绘制散点图
    save_gap(rand_num_gap, "output/Rand()%N_gap");

    std::cout << "Successfully Calculated the Frequency And the Gap of Random Numbers" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "Start Test" << std::endl;

    // 测评工具1：卡方拟合优度检验
    rand_n &= Chi2_test(rand_num_frequency, rand_num_count);

    // 测评工具2：科尔莫戈洛夫-斯米尔诺夫（KS）检验
    rand_n &= ks_test(rand_num_frequency, rand_num_count);

    // 测评工具3：自相关检验
    rand_n &= autocorrelation_test(rand_num_list, 10);

    // 测评工具4：游程检验
    rand_n &= runs_test(rand_num_list);

    // 测评工具5：间隔分布检验
    rand_n &= gap_test(rand_num_gap, rand_num_max);

    // 测评工具6：NIST检验
    nist_test(rand_num_list);

    std::cout << "--------------------------------" << std::endl;
    std::cout << "Successfully Tested" << std::endl;

    // 使用梅森旋转获取[0,N-1]上均匀分布的随机数

    std::cout << "================================" << std::endl;
    std::cout << "Start Generating Random Numbers by Mt19937" << std::endl;

    // 获取以rand()%N方式生成的随机数数组
    rand_num_list = get_rand_num_by_mt19937(rand_num_max, rand_num_count);

    std::cout << "Successfully Generated Random Numbers by Mt19937" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "Start Calculate the Frequency And the Gap of Random Numbers" << std::endl;

    // 统计[0,N-1]中每一个数在随机数数组中的出现频率
    rand_num_frequency = get_frequency(rand_num_list, rand_num_max);

    // 输出结果为csv并利用python脚本绘制折线图
    save_frequency(rand_num_frequency, "output/Mt19937_frequency");

    // 统计[0,N-1]中每一个数在随机数数组中重复的间隔，其中rand_num_gap的第index项为index数的不同距离的map，键为距离，值为出现频率，其中距离为两个相同数的index相减
    rand_num_gap = get_gap(rand_num_list, rand_num_max);

    // 输出结果为csv并利用python脚本绘制散点图
    save_gap(rand_num_gap, "output/Mt19937_gap");

    std::cout << "Successfully Calculated the Frequency And the Gap of Random Numbers" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "Start Test" << std::endl;

    // 测评工具1：卡方拟合优度检验
    mt19937 &= Chi2_test(rand_num_frequency, rand_num_count);

    // 测评工具2：科尔莫戈洛夫-斯米尔诺夫（KS）检验
    mt19937 &= ks_test(rand_num_frequency, rand_num_count);

    // 测评工具3：自相关检验
    mt19937 &= autocorrelation_test(rand_num_list, 10);

    // 测评工具4：游程检验
    mt19937 &= runs_test(rand_num_list);

    // 测评工具5：间隔分布检验
    mt19937 &= gap_test(rand_num_gap, rand_num_max);

    std::cout << "--------------------------------" << std::endl;
    std::cout << "Successfully Tested" << std::endl;

    // 使用Box-Muller变换，将两个[0,1)上均分分布的随机数变换为标准正态随机数
    std::cout << "================================" << std::endl;
    std::cout << "Start Generating Random Numbers by Box-Muller" << std::endl;

    // 获取标准正态随机数
    constexpr double mean = 0.0;
    constexpr double variance = 1.0;
    std::vector<double> rand_num_normal = get_rand_num_by_Box_Muller(0 ,1 , rand_num_count);

    std::cout << "Successfully Generated Random Numbers by Box-Muller" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "Start Drawing the Distribution Map" << std::endl;

    // 输出结果为csv并利用python脚本绘制归一化直方图与理论曲线
    save_normal(rand_num_normal, mean, variance, "output/normal");

    std::cout << "Successfully Drawn the Distribution Map" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "Start Test" << std::endl;

    // 利用卡方检验，将（-∞，+∞）分割成100个等概率区间，这样每个区间的随机数出现频率应相等，通过计算卡方值来判断是否成立。
    normal &= normal_test(rand_num_normal, mean, variance);

    std::cout << "--------------------------------" << std::endl;
    std::cout << "Successfully Tested" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "Summary" << std::endl;
    std::cout << "rand()%N is " << (rand_n ? "Good" : "BAD") << std::endl;
    std::cout << "mt19937()%N is " << (mt19937 ? "Good" : "BAD") << std::endl;
    std::cout << "normal is " << (normal ? "Good" : "BAD") << std::endl;
    std::cout << "==============================" << std::endl;

    return mt19937;

}

std::vector<int> get_rand_num_by_rand(const int rand_num_max, const int rand_num_count) {

    // 设置随机数种子
    srand(time(nullptr));

    std::vector<int> rand_num_list(rand_num_count);
    for (int i = 0; i < rand_num_count; i++) {rand_num_list[i] = rand() % rand_num_max; }
    return rand_num_list;
}

std::vector<double> get_frequency(const std::vector<int>& rand_num_list, const int rand_num_max) {

    // 生成频率数组并置0
    std::vector<double> rand_num_frequency(rand_num_max);
    for (int i = 0; i < rand_num_max; i++) {rand_num_frequency[i] = 0; }

    // 遍历rand_num_list，并在对应项上计数
    const size_t rand_num_count = rand_num_list.size();
    const double frequency_add_per_appear = static_cast<double>(1) / static_cast<double>(rand_num_count);
    for (int i = 0; i < rand_num_count; i++) {rand_num_frequency[rand_num_list[i]] += frequency_add_per_appear; }

    return rand_num_frequency;
}

void save_frequency(const std::vector<double>& rand_num_frequency, const std::string& filename) {

    // 输出为csv
    std::ofstream file(filename + ".csv");
    file << "数值,频率\n";
    for (size_t i = 0; i < rand_num_frequency.size(); ++i) {
        file << i << "," << rand_num_frequency[i] << "\n";
    }

    file.close();

    // 调用python脚本绘图
    std::system(("python plot_frequency.py " + filename).c_str());

}

std::map<int,int> get_gap(const std::vector<int>& rand_num_list, const int rand_num_max) {

    // 生成间隔数组，和上一次出现数字的index的数组，-1代表从未出现过
    std::map<int,int> rand_num_gap;
    std::vector<int> num_appear_index(rand_num_max);
    for (int i = 0; i < rand_num_max; i++) {num_appear_index[i] = -1; }

    // 遍历rand_num_list，并在对应项上计数
    const size_t rand_num_count = rand_num_list.size();
    for (int i = 0, rand_num = 0, gap = 0; i < rand_num_count; i++) {

        // 如果出现过则修改rand_num_gap，不然直接改变num_appear_index，
        rand_num = rand_num_list[i];
        if (num_appear_index[rand_num]!=-1) {

            // 计算间隔并重置出现位置
            gap = i - num_appear_index[rand_num];
            num_appear_index[rand_num] = i;

            // 查询数组中是否出现过该间隔，出现过则加1，不然则添加键值对
            if (rand_num_gap.count(gap)) {
                rand_num_gap[gap] ++;
            } else {
                rand_num_gap[gap] = 1;
            }

        } else {num_appear_index[rand_num] = i; }

    }

    return rand_num_gap;
}

void save_gap(const std::map<int,int> &rand_num_gap, const std::string& filename) {

    // 输出为csv
    std::ofstream file(filename + ".csv");
    file << "间隔,出现次数\n";
    for (auto &p : rand_num_gap) {
        file << p.first << "," << p.second << std::endl;
    }

    file.close();

    // 调用python脚本绘图
    std::system(("python plot_gap.py " + filename).c_str());

}

bool Chi2_test(const std::vector<double>& rand_num_frequency, const int rand_num_count) {

    // 获取随机数种类数
    const size_t rand_num_max = rand_num_frequency.size();
    const double expected = 1.f/static_cast<float>(rand_num_max);
    double chi2 = 0;

    for (int i = 0; i < rand_num_max; i++) {
        chi2 += (rand_num_frequency[i] - expected) * (rand_num_frequency[i] - expected) * static_cast<float>(rand_num_count) / expected;
    }

    // 获取卡方上限值
    const double critical = get_Chi2(rand_num_max-1);

    std::cout << "--------------------------------" << std::endl;
    std::cout << "1: Chi2_test " << std::endl;
    std::cout << "Chi2: " << chi2 <<std::endl;
    std::cout << "Critical value in alpha = 0.05: " << critical << std::endl;
    std::cout << "Uniform distribution? " << (chi2 <= critical ? "YES" : "NO") << "\n";

    return chi2 <= critical;

}

double get_Chi2(const unsigned int df) {

    // 默认 alpha 为0.05
    // 优先查表
    if (df >= 1 && df <= 100) {
        constexpr double Chi2[] = {
            // df: 1, 2, 3, ..., 100 (索引 0 为 df=1)
            3.841, 5.991, 7.815, 9.488, 11.070, 12.592, 14.067, 15.507, 16.919, 18.307,
            19.675, 21.026, 22.362, 23.685, 24.996, 26.296, 27.587, 28.869, 30.144, 31.410,
            32.671, 33.924, 35.172, 36.415, 37.652, 38.885, 40.113, 41.337, 42.557, 43.773,
            44.985, 46.194, 47.400, 48.602, 49.802, 50.998, 52.192, 53.384, 54.572, 55.758,
            56.942, 58.124, 59.304, 60.481, 61.656, 62.830, 64.001, 65.171, 66.339, 67.505,
            68.669, 69.832, 70.993, 72.153, 73.311, 74.468, 75.624, 76.778, 77.931, 79.082,
            80.232, 81.381, 82.529, 83.675, 84.821, 85.965, 87.108, 88.250, 89.391, 90.531,
            91.670, 92.808, 93.945, 95.081, 96.217, 97.351, 98.484, 99.617, 100.749, 101.879,
            103.010, 104.139, 105.267, 106.395, 107.522, 108.648, 109.773, 110.898, 112.022, 113.145,
            114.268, 115.390, 116.511, 117.632, 118.752, 119.871, 120.990, 122.108, 123.225, 124.342
        };
        return Chi2[df - 1];
    }

    // 对于 df > 100，使用近似公式（Wilson-Hilferty）
    return df * std::pow(1.0 - 2.0/(9*df) + 1.644854 * std::sqrt(2.0/(9*df)), 3);
}

bool ks_test(const std::vector<double> &rand_num_frequency, const int rand_num_count) {

    // 获取随机数种类数
    const size_t rand_num_max = rand_num_frequency.size();

    // 经验CDF: 到i为止的比例
    std::vector<double> Fn(rand_num_max);
    double cum = 0.0;
    for (int i = 0; i < rand_num_max; ++i) {
        cum += rand_num_frequency[i];
        Fn[i] = cum;
    }

    // 理论CDF: F(i) = (i+1)/N
    auto F_theory = [rand_num_max](const int i) { return static_cast<double>(i + 1) / static_cast<double>(rand_num_max); };

    // 计算离散KS统计量 D = max(|Fn(i) - F(i)|)
    double D = 0.0;
    for (int i = 0; i < rand_num_max; ++i) {
        const double F_i = F_theory(i);
        D = std::max(D, std::abs(Fn[i] - F_i));
    }

    // 临界值近似，适用于离散分布的 Kolmogorov 分布, α=0.05。公式: D_crit = 1.3581 / sqrt(n)
    const double D_critical = 1.3581 / std::sqrt(rand_num_count);

    std::cout << "--------------------------------" << std::endl;
    std::cout << "2: ks_test " << std::endl;
    std::cout << "KS statistic D: " << D <<std::endl;
    std::cout << "Critical value in alpha = 0.05: " << D_critical << std::endl;
    std::cout << "Uniform distribution? " << ((D <= D_critical) ? "YES" : "NO") << "\n";

    return D <= D_critical;

}

bool autocorrelation_test(const std::vector<int>& rand_num_list, const int max_lag) {

    // 获取随机数数组大小
    const size_t rand_num_count = rand_num_list.size();

    // 计算均值
    double mean = 0.0;
    for (const int i : rand_num_list) mean += i;
    mean /= static_cast<double>(rand_num_count);

    // 计算总方差*rand_num_count，即相关系数分母
    double denom = 0.0;
    for (const int i : rand_num_list) denom += (i - mean) * (i - mean);
    if (denom == 0.0) { return false; }

    // 计算正态分布临界值
    constexpr double z_alpha2 = 1.96;
    const double se = 1.0 / std::sqrt(rand_num_count);
    double crit_lower = -z_alpha2 * se;
    double crit_upper =  z_alpha2 * se;

    // 计算每个滞后的自相关系数
    bool is_random = true;
    int k;
    for (k = 1; k <= max_lag; ++k) {
        double num = 0.0;
        for (int i = 0; i < rand_num_count - k; ++i) {
            num += (rand_num_list[i] - mean) * (rand_num_list[i + k] - mean);
        }
        const double rk = (num / static_cast<double>(rand_num_count - k)) / (denom / static_cast<double>(rand_num_count));  // 用 n-k 归一化分子，用 n 归一化分母

        // 判断
        if(crit_lower > rk || rk > crit_upper) {
            is_random = false;
            break;
        };
    }

    std::cout << "--------------------------------" << std::endl;
    std::cout << "3: autocorrelation_test " << std::endl;
    std::cout << "Uniform distribution? " << ((is_random) ? "YES" : "NO") << "\n";
    if (!is_random) {
        std::cout << "Which lag make them no uniform ? " << k << std::endl;
    }

    return is_random;
}

bool runs_test(const std::vector<int>& rand_num_list) {

    // 构建方向序列，相等的情况在下面考虑
    std::vector<int> directions;
    for (size_t i = 0; i < rand_num_list.size() - 1; ++i) {
        if (rand_num_list[i] < rand_num_list[i+1]) {
            directions.push_back(1);   // 上升
        } else if (rand_num_list[i] > rand_num_list[i+1]) {
            directions.push_back(-1);  // 下降
        } else if (directions.empty() or directions.back() != 0) {
            directions.push_back(0);   // 不变则置0，且不会出现连续的0
        }
    }

    // 清理开头，结尾的零
    if (directions[0] == 0) {directions[0] = directions[1];}
    if (directions[directions.size()-1] == 0) {directions[directions.size()-1] = directions[directions.size()-2];}
    for (size_t i = 0; i < directions.size(); ++i) {
        if (directions[i] == 0) {
            // 零两侧游程相同则合并
            if (directions[i-1] == 1 and directions[i+1] == 1) {directions[i] = 1;}
            if (directions[i-1] == -1 and directions[i+1] == -1) {directions[i] = -1;}
        }
    }

    directions.erase(std::remove_if(directions.begin(), directions.end(), [](int x) { return x == 0; }), directions.end());

    // 获取有效方向对的数量，当有效方向对为0是该数组恒为常数，显然不随机
    const size_t m = directions.size();
    if (m == 0) { return false; }

    // 统计游程数
    double R = 1.0;
    for (size_t i = 1; i < m; ++i) {
        if (directions[i] != directions[i-1]) {
            R += 1.0;
        }
    }

    // 期望值与方差
    const auto N_prime = static_cast<double>(m + 1);
    const double E_R = (2.0 * N_prime - 1.0) / 3.0;
    const double Var_R = (16.0 * N_prime - 29.0) / 90.0;

    // 标准化统计量，alpha为0.05时Z的上下限应为±1.96
    const double Z = (R - E_R) / std::sqrt(Var_R);

    std::cout << "--------------------------------" << std::endl;
    std::cout << "4: runs_test " << std::endl;
    std::cout << "Actual value of Runs: " << R << std::endl;
    std::cout << "Expected value of Runs: " << E_R << std::endl;
    std::cout << "Variance of Runs: " << Var_R << std::endl;
    std::cout << "Z value of Runs: " << Z << std::endl;
    std::cout << "Uniform distribution? " << ((std::abs(Z)<1.96) ? "YES" : "NO") << "\n";

    return std::abs(Z)<1.96;
}

bool gap_test(std::map<int,int>& rand_num_gap, const int rand_num_max) {

    // 理论分布：p = 1/N, 几何分布 P(d) = p * (1-p)^(d-1)
    double p = 1.0 / rand_num_max;

    // 期望频数总和应等于观测总数
    int total_gaps = 0;
    for (const auto [fst, snd] : rand_num_gap) {total_gaps += snd;}

    // 合并低频类别，保证每个类别的期望频数 >= 5
    constexpr int MIN_EXPECTED = 5;
    std::vector<double> observed;
    std::vector<double> expected;

    int max_gap = 1;
    // 首先找到数据中的最大间隔，用于决定合并的上界
    for (auto&[fst, snd] : rand_num_gap) {
        max_gap = std::max(max_gap, fst);
    }

    double cum_obs = 0.0;
    double cum_exp = 0.0;
    int current_gap = 1;
    while (current_gap <= max_gap) {
        double exp_gap = total_gaps * p * std::pow(1.0 - p, current_gap - 1);
        double obs_gap = rand_num_gap.count(current_gap) ? rand_num_gap[current_gap] : 0;

        // 累加直到期望频数达到最低阈值，或者到达最后
        if (cum_exp + exp_gap >= MIN_EXPECTED || current_gap == max_gap) {
            // 如果之前有累积，先存入累积组
            if (cum_exp > 0) {
                // 将当前加入累积，一起作为一个组
                observed.push_back(cum_obs + obs_gap);
                expected.push_back(cum_exp + exp_gap);
                cum_obs = 0.0;
                cum_exp = 0.0;
            } else {
                observed.push_back(obs_gap);
                expected.push_back(exp_gap);
            }
            current_gap++;
        } else {
            // 期望太小，累积到后续组
            cum_obs += obs_gap;
            cum_exp += exp_gap;
            current_gap++;
        }
    }
    // 如果最后还有未弹出的累积（所有组都没达到阈值且没到max_d），强制弹出
    if (cum_exp > 0) {
        observed.back() += cum_obs;
        expected.back() += cum_exp;
    }

    // 计算卡方统计量
    double chi2 = 0.0;
    const size_t bins = observed.size();
    for (int i = 0; i < bins; ++i) {
        if (expected[i] > 0) {
            const double diff = observed[i] - expected[i];
            chi2 += (diff * diff) / expected[i];
        }
    }

    // 自由度 = bins - 1
    const double critical = get_Chi2(bins - 1);

    std::cout << "--------------------------------" << std::endl;
    std::cout << "5: gap_test " << std::endl;
    std::cout << "Totol gaps Kinds: " << bins << std::endl;
    std::cout << "Chi2: " << chi2 <<std::endl;
    std::cout << "Critical value in alpha = 0.05: " << critical << std::endl;
    std::cout << "Uniform distribution? " << (chi2 <= critical ? "YES" : "NO") << "\n";

    return true;
}

std::vector<int> get_rand_num_by_mt19937(const int rand_num_max, const int rand_num_count) {

    // 创建随机数数组
    std::vector<int> rand_num_list(rand_num_count);

    std::random_device rd;
    std::mt19937_64 gen(rd());

    // 定义分布 [0, N-1] 的整数均匀分布
    std::uniform_int_distribution<int> dist(0, rand_num_max - 1);

    // 生成样本随机数
    for (int i = 0; i < rand_num_count; ++i) {
        rand_num_list[i] = dist(gen);
    }
    return rand_num_list;

}

std::vector<double> get_rand_num_by_Box_Muller(const double mean, const double variance, const int rand_num_count) {

    std::vector<double> rand_num_list(rand_num_count);

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0); // [0,1) 浮点

    for (int i = 0; i < rand_num_count; i += 2) {
        double u1 = dist(gen);
        while (u1 == 0.0) u1 = dist(gen); // 避免log(0)
        const double u2 = dist(gen);
        const double z0 = std::sqrt(-2.0 * std::log(u1)) * std::cos(2.0 * M_PI * u2);
        const double z1 = std::sqrt(-2.0 * std::log(u1)) * std::sin(2.0 * M_PI * u2);

        rand_num_list[i] = mean + variance * z0;
        if (i + 1 < rand_num_count) {
            rand_num_list[i+1] = mean + variance * z1;
        }
    }

    return rand_num_list;

}

void save_normal(const std::vector<double>& rand_num_normal, const double mean, const double variance, const std::string& filename) {

    // 输出为csv
    std::ofstream file(filename + ".csv");
    file << "value\n";
    for (const double i : rand_num_normal) {
        file << i << "\n";
    }

    file.close();

    // 调用python脚本绘图
    std::system(("python plot_normal.py " + (filename) + " " + std::to_string(mean) + " " + std::to_string(variance)).c_str());

}

double normal_inv(const double p, const double mean, const double variance) {

    // 逆正态累积分布函数
    // 给定概率 p，返回满足 Φ(z) = p 的 z 值

    // 概率应在0~1之间
    if (p <= 0.0) return static_cast<double>(-INFINITY);
    if (p >= 1.0) return static_cast<double>(INFINITY);

    const double target = 2.0 * p - 1.0;
    double lo = -10.0, hi = 10.0;
    while (hi - lo > 1e-12) {
        const double mid = (lo + hi) / 2.0;
        const double val = std::erf(mid / std::sqrt(2.0));
        if (val < target) lo = mid;
        else hi = mid;
    }
    return mean + variance * (lo + hi) / 2.0;

}

bool normal_test(const std::vector<double>& rand_num_normal, const double mean, const double variance) {

    // 卡方分组数量
    constexpr int k = 100;

    // 随机数总数
    int n = rand_num_normal.size();

    // 确认每个分组的概率区间
    std::vector<double> edges(k + 1);
    edges[0] = -INFINITY;
    for (int i = 1; i < k; ++i) {
        const double p = static_cast<double>(i) / k;
        edges[i] = normal_inv(p, mean, variance);
    }
    edges[k] = INFINITY;

    // 确认每个分组里的随机数出现次数
    std::vector<int> obs(k, 0);
    for (const double x : rand_num_normal) {
        for (int i = 0; i < k; ++i) {
            if (x >= edges[i] && x < edges[i+1]) {
                obs[i]++;
                break;
            }
        }
    }

    // 理论出现次数
    const double expected_freq = static_cast<double>(n) / k;

    // 计算卡方统计量
    double chi2 = 0.0;
    for (int i = 0; i < k; ++i) { chi2 += ((obs[i] - expected_freq) * (obs[i] - expected_freq)) / expected_freq; }

    // 获取卡方上限值
    const double critical = get_Chi2(k-1);

    std::cout << "--------------------------------" << std::endl;
    std::cout << "1: normal_test " << std::endl;
    std::cout << "Chi2: " << chi2 <<std::endl;
    std::cout << "Critical value in alpha = 0.05: " << critical << std::endl;
    std::cout << "Normal distribution? " << (chi2 <= critical ? "YES" : "NO") << "\n";

    return chi2 <= critical;

}

void nist_test(const std::vector<int> &rand_num_list) {

    std::cout << "--------------------------------" << std::endl;
    std::cout << "6: nist_test " << std::endl;

    // 将随机数组的最低位转为比特串
    std::ofstream out("output/nist_rng.txt");
    for (const int x : rand_num_list)
        out << (x & 1 ? '1' : '0');
    out.close();

    std::ofstream cmd("output/nist_cmd.txt");
    cmd << "0\noutput/nist_rng.txt\n1\n0\n10\n" << std::endl;

    // 调用assess.exe
    std::cout << "Starting assess.exe " << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::system((std::string("assess.exe ") + std::to_string(static_cast<int>(rand_num_list.size()/10)) + std::string(" < output/nist_cmd.txt" )).c_str());
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Successfully Run assess.exe " << std::endl;
    std::cout << "Please Find the Result of NIST Test in the ./experiments/AlgorithmTesting/finalAnalysisReport.txt" << std::endl;

}
