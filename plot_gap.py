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

    # 绘图
    plt.plot(df[cols[0]], df[cols[1]], linestyle='-', color='red')
    plt.title("随机数重复出现的间隔的分布")
    plt.xlabel(cols[0])
    plt.ylabel(cols[1])
    plt.grid(True, alpha=0.3)
    plt.savefig(sys.argv[1] + ".png", dpi=400)

    # 获取上下限并绘图
    # min_val = df[cols[1]].min()
    # max_val = df[cols[1]].max()
    # df = df.sort_values(cols[2])
    # plt.scatter(
    #     df[cols[0]],
    #     df[cols[1]],
    #     s=df[cols[2]]*0.5,
    #     c=df[cols[2]],
    #     cmap = 'plasma',
    #     marker='o',
    #     alpha=0.7,
    # )
    # plt.ylim(0,1.1*max_val)
    # plt.title("随机数重复出现的间隔的分布")
    # plt.xlabel(cols[0])
    # plt.ylabel(cols[1])
    # plt.savefig(sys.argv[1] + ".png", dpi=400)
