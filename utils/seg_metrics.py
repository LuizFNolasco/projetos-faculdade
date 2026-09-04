import numpy as np
import json


def get_iou (pred_mask, true_mask):
    """
    This function computes the Intersection over Union (IoU) between the predicted mask and the true mask.    
    
    :param pred (np.array): predicted mask (binary array)        
    :param true_mask (np.array): true mask (binary array)

    :return: float: IoU value
    """
    intersection = (pred_mask & true_mask).sum()
    union = (pred_mask | true_mask).sum()
    return intersection / union if union != 0 else 0.0


def get_dice(pred_mask, true_mask):
    """
    This function computes the DICE coefficient between the predicted mask and the true mask.    

    :param pred (np.array): predicted mask (binary array)        
    :param true_mask (np.array): true mask (binary array)

    :return: float: DICE coefficient value    
    """    
    intersection = (pred_mask & true_mask).sum()
    den = pred_mask.sum() + true_mask.sum()
    return 2 * intersection / den if den != 0 else 0.0
    


def get_pixel_accuracy(pred_mask, true_mask):
    """
    This function calculates the Pixel Accuracy between the predicted mask and the true mask.

    :param pred (np.array): predicted mask (binary array)        
    :param true_mask (np.array): true mask (binary array)

    :return: float: Pixel Accuracy value
    """
    correct = (pred_mask == true_mask).sum()
    total = pred_mask.size
    return correct / total


def compute_metrics(pred_mask, true_mask, metrics=("iou", "dice", "pixel_accuracy"), 
                    verbose=False, save_path=None):
    """
    This function computes the metrics specified in the metrics list between the predicted mask and the true mask.

    :param pred (np.array): predicted mask (binary array)        
    :param true_mask (np.array): true mask (binary array)
    :param metrics (list): list of metrics to compute. Default: ("iou", "dice", "pixel_accuracy")
    :param verbose (bool): if True, print the computed metrics
    :param save_path (str): if specified, save the computed metrics in the specified path as a JSON file

    :return: dict: dictionary with the computed metrics
    """
    computed_metrics = {}
    if "iou" in metrics:
        computed_metrics["iou"] = get_iou(pred_mask, true_mask)
    if "dice" in metrics:
        computed_metrics["dice"] = get_dice(pred_mask, true_mask)
    if "pixel_accuracy" in metrics:
        computed_metrics["pixel_accuracy"] = get_pixel_accuracy(pred_mask, true_mask)

    if verbose:
        for metric, value in computed_metrics.items():
            print(f"- {metric}: {value:.2f}")

    if save_path is not None:
        with open(save_path, "w") as f:
            json.dump(computed_metrics, f, indent=4)

    return computed_metrics


if __name__ == "__main__":
    
    # Use case example

    print("-" * 50)
    print("- Use case examples for seg_metrics.py")
    print("-" * 50)
    pred_mask = np.array([[1, 1, 0, 0],
                        [0, 1, 1, 0],
                        [0, 0, 1, 1],
                        [0, 0, 0, 1]])

    true_mask = np.array([[1, 1, 0, 0],
                        [0, 1, 1, 1],
                        [0, 0, 1, 1],
                        [0, 0, 0, 0]])

    compute_metrics(pred_mask, true_mask, verbose=True)
    print("-" * 50)
