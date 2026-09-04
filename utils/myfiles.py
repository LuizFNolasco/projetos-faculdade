
import os

def safe_mkdir(folders, base_folder=None, verbose=True):
    """
    This function creates folders in safe way, that is, it only creates a new folder if it doesn't exist.

    Parameters
    ----------
    folders : list or string
        The folder(s) that will be created
    base_folder : string
        The base folder (if applicable)
    """
    if not isinstance(folders, list):
        folders = [folders]

    for fold in folders:
        if base_folder is not None:
            fold = os.path.join(base_folder, fold)

        if not os.path.isdir(fold):
            os.makedirs(fold)
            if verbose:
                print("- {} created".format(fold))
        else:
            if verbose:
                print("- {} already exists!".format(fold))


def get_folders_name(path):
    """
    This function returns the name of all folders within a given path
    Parameters
    ----------
    path : string
        The path to get the folders name

    Returns
    -------
    folder_names : list
        A list with the folders name
    """
    names = [d for d in os.listdir(path) if os.path.isdir(os.path.join(path, d))]
    return names