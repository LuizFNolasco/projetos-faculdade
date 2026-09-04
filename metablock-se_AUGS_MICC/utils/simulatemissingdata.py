import os
import numpy as np
import pandas as pd

def remove_samples(df, missing_percentage):
    np.random.seed(42)
    mask = np.random.rand(len(df), len(df.columns)) < (1 - missing_percentage)
    new_values = pd.Series([np.nan for _ in df.columns], index=df.columns)
    df[df.columns] = df[df.columns].where(mask, other=new_values, axis=1)
    return df

def simulate_missing_data(df, save_folder, dataset, encoder_function,
                            encoder_name:str, missing_percentages=[0],
                            save_function=None, model=None):
    os.makedirs(save_folder, exist_ok=True)
    all_features = []
    for missing_percentage in missing_percentages:

        df_with_missing = df.copy()
        if (missing_percentage > 0):
            df_with_missing = remove_samples(df_with_missing, missing_percentage/100)

        encoded = encoder_function(df_with_missing)
        if missing_percentage != 0:
            # makes sure every csv has the same columns
            for feature in all_features:
                if feature not in encoded.columns:
                    encoded[feature] = 0
        else:
            all_features = encoded.columns

        if save_function is not None:
            save_function(encoded, missing_percentage, False)
        else:
            encoded.to_csv(os.path.join(save_folder, f"{dataset}-{model}-{encoder_name}-missing-{missing_percentage}.csv"), index=False)