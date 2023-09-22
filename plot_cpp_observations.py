import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import sys

plt.rcParams["font.family"] = "tahoma"
plt.rcParams['font.size'] = 11

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print('File not given')
        exit(-1)
    df = pd.read_csv(sys.argv[1], names=['s', 'size_u', 'size_v', 'cos_org', 'cos_fmh'], delimiter=' ')
    fig, axs = plt.subplots(2, 2, sharex=True)
    a, b = min(df['size_v'].tolist()), max(df['size_v'].tolist())

    for s, coord in zip([0.0001, 0.001, 0.01, 0.1], [ [0,0], [0,1], [1,0], [1,1] ]):
        df2 = df[ (df['s'] == s) ]
        axs[coord[0],coord[1]].scatter(df2['size_v'], 100*(df2['cos_org']-df2['cos_fmh'])/df2['cos_org'], s=1, color='#0077b6')
        axs[coord[0],coord[1]].plot( [a, b], [0, 0], color='#03045e')
        axs[coord[0],coord[1]].set_title(f'FMH Scale Factor {s}', fontsize=11)
        #axs[coord[0],coord[1]].set_ylim(-100,100)

    for ax in axs.flat:
        ax.set(xlabel='Number of kmers in variable set', ylabel='% diff from true value')

    fig.suptitle('Estimated vals for various scale factors', fontsize=11)
    plt.subplots_adjust(hspace=0.4, wspace=0.4)

    plt.savefig('diff_of_metrics.pdf')
    plt.show()
