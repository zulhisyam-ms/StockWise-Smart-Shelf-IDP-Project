import streamlit as st
import pandas as pd

# Initial data
if 'data' not in st.session_state:
    st.session_state.data = pd.DataFrame(columns=["Water", "Snack", "Instant Noodle"])

# Function to add values to the table
def add_values():
    new_value1 = st.number_input("Enter value for Water", value=0, key='Water')
    new_value2 = st.number_input("Enter value for Snack", value=0, key='Snack')
    new_value3 = st.number_input("Enter value for Instant Noodle", value=0, key='Instant Noodle')
    if st.button("Add to Table"):
        new_row = pd.DataFrame({"Water": [new_value1], "Snack": [new_value2], "Instant Noodle": [new_value3]})
        st.session_state.data = pd.concat([st.session_state.data, new_row], ignore_index=True)

# Function to plot the line chart
def plot_chart():
    if st.button("Plot Line Chart"):
        st.line_chart(st.session_state.data)

st.title("Analysis Website for total sale")

# Adding values to the table
add_values()

# Display the table
st.write("Table of Values")
st.write(st.session_state.data)

# Plotting the line chart
plot_chart()
