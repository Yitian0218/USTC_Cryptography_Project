import pandas as pd
import matplotlib.pyplot as plt
import sys

if __name__ == "__main__":

    # 切换中文字体，不然图像的抬头等可能输出方块
    plt.rcParams['font.sans-serif'] = ['STSong']
    plt.rcParams['axes.unicode_minus'] = False

    # 读取数据表格
    df = pd.read_csv(sys.argv[1] + ".csv")
    cols = df.columns.tolist()
    min_val = df[cols[1]].min()
    max_val = df[cols[1]].max()

    # 绘图
    plt.plot(df[cols[0]], df[cols[1]], linestyle='-', color='red')
    plt.ylim(0.5*min_val,1.5*max_val)
    plt.title("随机数的分布频率")
    plt.xlabel(cols[0])
    plt.ylabel(cols[1])
    plt.grid(True, alpha=0.3)
    plt.savefig(sys.argv[1] + ".png", dpi=400)