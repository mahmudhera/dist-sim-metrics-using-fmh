import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import sys

plt.rcParams["font.family"] = "tahoma"
plt.rcParams['font.size'] = 11

scale_factors = [0.0001, 0.001, 0.01, 0.1];
#scale_factors = [0.01, 0.1, 0.95, 1.0]
ylim = 50
clip_y = False

if __name__ == "__main__":
    print('Usage: python script_name.py obs_filename pdf_filename ylim clip_y')
    if len(sys.argv) < 5:
        print('File(s) not given')
        exit(-1)

    if sys.argv[4]=='true' or sys.argv[4]=='True' or sys.argv[4]==True:
        ylim = int(sys.argv[3])
        clip_y = True

    df = pd.read_csv(sys.argv[1], names=['s', 'size_u', 'size_v', 'cos_org', 'cos_fmh'], delimiter=' ')
    fig, axs = plt.subplots(2, 2, sharex=True)
    a, b = min(df['size_v'].tolist()), max(df['size_v'].tolist())

    for s, coord in zip(scale_factors, [ [0,0], [0,1], [1,0], [1,1] ]):
        df2 = df[ (df['s'] == s) ]
        axs[coord[0],coord[1]].scatter(df2['size_v'], 100*(df2['cos_org']-df2['cos_fmh'])/df2['cos_org'], s=1, color='#0077b6')
        axs[coord[0],coord[1]].plot( [a, b], [0, 0], color='#03045e')
        axs[coord[0],coord[1]].set_title(f'FMH Scale Factor {s}', fontsize=11)
        if clip_y:
            axs[coord[0],coord[1]].set_ylim(-ylim,ylim)

    for ax in axs.flat:
        ax.set(xlabel='Number of kmers in variable set', ylabel='% diff from true value')

    fig.suptitle('Estimated vals for various scale factors', fontsize=11)
    plt.subplots_adjust(hspace=0.4, wspace=0.4)

    plt.savefig(sys.argv[2])
    plt.show()
