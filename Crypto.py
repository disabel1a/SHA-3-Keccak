import json
import matplotlib.pyplot as plt
from pathlib import Path

# crypto
path = Path("C:/Users/ivmak/OneDrive/Рабочий стол/SHA_3_json");

shift = 128;

for file in path.rglob("*"):
    print(file)

    if(not file.name.find(".json")):
        continue
    
    with open(file, 'r') as file:
        data = json.load(file)
    
    values = data['values']
    name = data['name']
    xPoint = int(((len(values) - 1) / 2) * 5)
    size = len(values)

    # crypto
    plt.plot(values)
    plt.xlabel('Iteration')
    plt.ylabel('Step')
    
    plt.title(name)
    plt.grid(True)
    plt.show()