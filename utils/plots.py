
from matplotlib import pyplot as plt


def multiple_subplots(data, titles, colors, title=None, figsize=(21, 7), save_path=None, show=True,
                      flags=None):
    """
    This function plots a sequence of `n` subplots in the same figure. Each data is plot in a different row, but in the
    same column

    Parameters
    ----------
    data : list
        A list of arrays-like to be plotted
    titles : list
        A list of string that will be the tittles of each subplot. Obviously, it must match with data
    colors : list
        A list of string tha will be the colors of each plot. Obviously, it must match with data and tittles
    title : string
        A string that will be the tittle of the whole plot. If None, the plot won't have a tittle
    figsize : tuple
        The size of each subplot
    save_path : string
        A string with the full path to save this plot. If None, the plot won't be saved.
    show : boolean
        A boolean to show or not the plot.
    flags: array-like
        This is an array like representing the GWA flags. If it's not None, we span red and yellow colors for flags
        equal to -10 and -8, respectively
    """

    fig, ax = plt.subplots(nrows=len(data), ncols=1, figsize=figsize)

    if len(data) > 1:
        for k, (d, t, c) in enumerate(zip(data, titles, colors)):
            ax[k].plot(d, linewidth=1, color=c)
            ax[k].set_title(t)
            plt.grid(True, color='gray', linestyle='dotted', linewidth=0.3)
    else:
        ax.plot(data[0], linewidth=1)
        ax.set_title(titles[0])
        plt.grid(True, color='gray', linestyle='dotted', linewidth=0.3)

    # Dealing with the flags (if applicable)
    if flags is not None:
        for t, f in enumerate(flags, 1):
            if f == -10:
                plt.axvspan(t, t + 1, facecolor='fuchsia', alpha=0.15)
            if f == -8:
                plt.axvspan(t, t + 1, facecolor='yellow', alpha=0.15)

    fig.subplots_adjust(hspace=.5)
    if title is not None:
        plt.suptitle(title)
    if save_path is not None:
        plt.savefig(save_path)
    if show:
        plt.show()

    plt.close(fig)


def multiple_plots_together(data, labels, colors, title=None, figsize=(21, 7), save_path=None,
                            show=True, config=None, flags=None, intensity_flag=None, quality_flag=None,
                            cte_point=80, ax=None):
    """
    This function plots `n` curves in the same plot. Unlike the `multiple_subplots` function, which plots all data in
    different row, in this function the plots are in the same x-y axis.
    Parameters
    ----------
    data : list
        A list of arrays-like to be plotted
    labels : list
        A list of string that will be the labels of each curve. Obviously, it must match with data
    colors : list
        A list of string tha will be the colors of each plot. Obviously, it must match with data and labels
    title : string
        A string that will be the tittle of the whole plot. If None, the plot won't have a tittle
    figsize : tuple
        The size of each subplot
    save_path : string
        A string with the full path to save this plot. If None, the plot won't be saved.
    show : boolean
        A boolean to show or not the plot.
    config: dict:
        A dict to control the marker and marker width for a scatter plot (if applicable)
    flags: array-like
        This is an array like representing the GWA flags. If it's not None, we span red and yellow colors for flags
        equal to -10 and -8, respectively
    """

    marker = "."
    marker_width = 0.2
    if config is not None:
        if "marker" in config:
            marker = config['marker']
        if "marker_width" in config:
            marker_width = config['marker_width']

    if ax is None:
        fig, ax = plt.subplots(nrows=1, ncols=1, figsize=figsize)

    for d, c in (zip(data, colors)):
        if len(d) == 2:
            ax.scatter(d[0], d[1], linewidth=marker_width, color=c, marker=marker)
        else:
            ax.plot(d, linewidth=1, color=c)

    # Dealing with the flags (if applicable)
    if flags is not None:
        for t, f in enumerate(flags, 1):
            if f == -10:
                plt.axvspan(t, t + 1, facecolor='fuchsia', alpha=0.15)
            elif f == -8:
                plt.axvspan(t, t + 1, facecolor='yellow', alpha=0.15)
            elif f == 0:
                plt.axvspan(t, t + 1, facecolor='gray', alpha=0.15)

    # Dealing with the intensity flags (if applicable)
    if intensity_flag is not None:
        x0, y0 = list(), list()
        x1, y1 = list(), list()
        for t, w in enumerate(intensity_flag, 1):
            if w == 1 or w == "HIGH":
                x1.append(t)
                y1.append(cte_point - 5)
            elif w == 0 or w == "LOW":
                x0.append(t)
                y0.append(cte_point - 5)
        ax.scatter(x0, y0, linewidth=3, color="springgreen", marker="s")
        ax.scatter(x1, y1, linewidth=3, color="brown", marker="s")

    # Dealing with the intensity flags (if applicable)
    if quality_flag is not None:
        xq1, yq1 = list(), list()
        xq0, yq0 = list(), list()
        for t, w in enumerate(quality_flag, 1):
            if w == 1:
                xq1.append(t)
                yq1.append(cte_point - 10)
            else:
                xq0.append(t)
                yq0.append(cte_point - 10)

        ax.scatter(xq1, yq1, linewidth=3, color="skyblue", marker="s")
        ax.scatter(xq0, yq0, linewidth=3, color="gold", marker="s")

    # ax.set_ylim(50)
    #plt.legend(labels, loc="upper left")
    plt.legend(labels, loc="lower right")
    plt.grid(True, color='gray', linestyle='dotted', linewidth=0.3)
    if title is not None:
        plt.title(title)
    if save_path is not None:
        plt.savefig(save_path)
    if show:
        plt.show()

    # if ax is None:
    plt.close(fig)