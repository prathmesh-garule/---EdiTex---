import pandas as pd
from sklearn.preprocessing import OneHotEncoder
from sklearn.linear_model import LinearRegression
from sklearn.impute import SimpleImputer
from comp_env import environment

# Initialize environment
env = environment()

# Fetch training data
X_train, y_train = env.get_training_data()

# One-hot encode the 'sym' column
encoder = OneHotEncoder(sparse_output=False)
X_train_sym_encoded = encoder.fit_transform(X_train[['sym']])

# Append encoded 'sym' to the original DataFrame
X_train_encoded = pd.concat([X_train.reset_index(drop=True), pd.DataFrame(X_train_sym_encoded, columns=encoder.get_feature_names_out(['sym']))], axis=1)

# Remove the 'sym' column as it is now encoded
X_train_encoded.drop(columns=['sym'], inplace=True)

# Handle missing values by filling with the mean
imputer = SimpleImputer(strategy='mean')
X_train_encoded[features] = imputer.fit_transform(X_train_encoded[features])

# Ensure X_train_encoded and y_train have the same number of rows
X_train_encoded = X_train_encoded.loc[y_train.index]

# Features used for training the model
features = ['volumeLastMinute', 'volumeLastHour', 'spread', 'mid'] + list(encoder.get_feature_names_out(['sym']))

# Step 1: Initialize a simple Linear Regression model
model = LinearRegression()

# Step 2: Train the model on the training dataset
model.fit(X_train_encoded[features], y_train['volumeNextHour'])

# Step 3: Define the prediction function
def predict(input_df, submission) -> pd.DataFrame:
    # One-hot encode 'sym' in the input_df
    input_sym_encoded = encoder.transform(input_df[['sym']])
    
    # Append encoded 'sym' to the input DataFrame
    input_encoded = pd.concat([input_df.reset_index(drop=True), pd.DataFrame(input_sym_encoded, columns=encoder.get_feature_names_out(['sym']))], axis=1)
    
    # Remove the original 'sym' column
    input_encoded.drop(columns=['sym'], inplace=True)
    
    # Handle missing values in the test data
    input_encoded[features] = imputer.transform(input_encoded[features])
    
    # Predict the next hour's volume
    submission['volumeNextHour'] = model.predict(input_encoded[features])
    
    return submission

# Step 4: Evaluate the model using the competition's environment
results = env.evaluate_model(predict, submit=True)
print(results)