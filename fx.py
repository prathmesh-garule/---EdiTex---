import pandas as pd
from sklearn.preprocessing import OneHotEncoder
from sklearn.linear_model import LinearRegression
from sklearn.model_selection import train_test_split
from sklearn.impute import SimpleImputer

# Assuming X_train, y_train are already loaded
X_train, y_train = env.get_training_data()

# One-hot encode 'sym' column
encoder = OneHotEncoder(sparse=False)
X_train_sym_encoded = encoder.fit_transform(X_train[['sym']])

# Append encoded 'sym' to the original DataFrame
X_train_encoded = pd.concat([X_train.reset_index(drop=True), 
                             pd.DataFrame(X_train_sym_encoded, columns=encoder.get_feature_names_out())], axis=1)

# Remove the original 'sym' column as it is now encoded
X_train_encoded.drop(columns=['sym'], inplace=True)

# Impute missing values in X_train_encoded and y_train using SimpleImputer or drop rows with NaNs
imputer = SimpleImputer(strategy='mean')
X_train_encoded = pd.DataFrame(imputer.fit_transform(X_train_encoded), columns=X_train_encoded.columns)

# Drop rows with NaNs in y_train
non_nan_mask = y_train.notna().all(axis=1)
X_train_encoded = X_train_encoded[non_nan_mask].reset_index(drop=True)
y_train = y_train[non_nan_mask].reset_index(drop=True)

# Split the data into training and validation sets
X_train_split, X_val_split, y_train_split, y_val_split = train_test_split(X_train_encoded, y_train, test_size=0.2, random_state=42)

# Train a simple linear regression model
model = LinearRegression()
model.fit(X_train_split, y_train_split['volumeNextHour'])

# Define the predict function
def predict(input_df, submission) -> pd.DataFrame:
    input_encoded = pd.concat([input_df.reset_index(drop=True), 
                               pd.DataFrame(encoder.transform(input_df[['sym']]), 
                                            columns=encoder.get_feature_names_out())], axis=1)
    input_encoded.drop(columns=['sym'], inplace=True)
    input_encoded = pd.DataFrame(imputer.transform(input_encoded), columns=input_encoded.columns)
    
    predictions = model.predict(input_encoded)
    submission['volumeNextHour'] = predictions
    return submission

# Evaluate the model
results = env.evaluate_model(predict, submit=True)
print(results)