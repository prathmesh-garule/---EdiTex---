import pandas as pd
import numpy as np
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import OneHotEncoder
from sklearn.impute import SimpleImputer
from comp_env import environment

# Initialize the environment
env = environment()

# Fetch training data
X_train, y_train = env.get_training_data()

# Reset indices to avoid mismatches
X_train.reset_index(drop=True, inplace=True)
y_train.reset_index(drop=True, inplace=True)

# One-hot encode the 'sym' column
encoder = OneHotEncoder(sparse_output=False)
X_train_sym_encoded = encoder.fit_transform(X_train[['sym']])

# Append encoded 'sym' to the original DataFrame
X_train_encoded = pd.concat([X_train, pd.DataFrame(X_train_sym_encoded, columns=encoder.get_feature_names_out(['sym']))], axis=1)

# Remove the original 'sym' column as it's now encoded
X_train_encoded.drop(columns=['sym'], inplace=True)

# Impute missing values
imputer = SimpleImputer(strategy='mean')
X_train_encoded = pd.DataFrame(imputer.fit_transform(X_train_encoded), columns=X_train_encoded.columns)

# Convert data types explicitly to avoid issues with numpy types
X_train_encoded = X_train_encoded.astype(float)
y_train = y_train.astype(float)

# Define features used for training
features = ['volumeLastMinute', 'volumeLastHour', 'spread', 'mid'] + list(encoder.get_feature_names_out(['sym']))

# Initialize and train the linear regression model
model = LinearRegression()
model.fit(X_train_encoded[features], y_train['volumeNextHour'])

# Prediction function
def predict(input_df, submission) -> pd.DataFrame:
    # Reset index for the input_df
    input_df.reset_index(drop=True, inplace=True)
    
    # One-hot encode 'sym' in the input_df
    input_sym_encoded = encoder.transform(input_df[['sym']])
    
    # Append encoded 'sym' to the input DataFrame
    input_encoded = pd.concat([input_df, pd.DataFrame(input_sym_encoded, columns=encoder.get_feature_names_out(['sym']))], axis=1)
    
    # Remove the 'sym' column after encoding
    input_encoded.drop(columns=['sym'], inplace=True)
    
    # Handle missing values in the test data
    input_encoded = pd.DataFrame(imputer.transform(input_encoded), columns=input_encoded.columns)
    
    # Convert data types explicitly to avoid issues with numpy types
    input_encoded = input_encoded.astype(float)
    
    # Predict the next hour's volume
    submission['volumeNextHour'] = model.predict(input_encoded[features])
    
    # Ensure the length of predictions matches the submission dataframe
    if len(submission) != len(input_encoded):
        raise ValueError(f"Length of submission ({len(submission)}) does not match the input data ({len(input_encoded)}).")
    
    return submission

# Evaluate the model
results = env.evaluate_model(predict, submit=True)
print(results)