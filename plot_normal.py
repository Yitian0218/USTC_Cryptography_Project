import sys
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm  # 用于生成理论正态曲线

if __name__ == "__main__":

    # 切换中文字体，不然图像的抬头等可能输出方块
    plt.rcParams['font.sans-serif']     = ['STSong']
    plt.rcParams['axes.unicode_minus']  = False

    # 读取数据表格
    df      = pd.read_csv(sys.argv[1] + ".csv")
    cols = df.columns.tolist()

    # 设置分布参数
    data    = df[cols[0]].dropna().values.ravel()
    mu      = float(sys.argv[2])
    sigma   = float(sys.argv[3])

    # 绘制
    plt.figure(figsize=(10, 6), dpi=200)

    # 绘制归一化直方图 (density=True 使直方图面积为1，可与概率密度函数比较)
    plt.hist(data, bins=50, density=True, alpha=0.7, color='skyblue', edgecolor='black', label='样本分布')

    # 绘制理论正态曲线
    x = np.linspace(mu - 4 * sigma, mu + 4 * sigma, 200)
    y = norm.pdf(x, loc=mu, scale=sigma)
    plt.plot(x, y, 'r-', linewidth=2, label=f'理论正态 N({mu:.2f}, {sigma:.2f}²)')
    plt.title("正态分布随机数检验")
    plt.xlabel("数值")
    plt.ylabel("概率密度")
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()

    # 保存
    plt.savefig(sys.argv[1] + '.png', dpi=400)