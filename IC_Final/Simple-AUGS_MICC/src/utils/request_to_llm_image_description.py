import requests

def request_to_ollama(llm_api_server_host:str="localhost", llm_api_server_port:str=11434, prompt_message:str="You are an experienced endoscopy research assistant. Given the following base64‑encoded endoscopic image of the gastrointestinal mucosa, write one natural, flowing paragraph that describes only its visual features—covering overall mucosal topography (e.g., flat, nodular, elevated, or depressed), surface texture, border definition of any distinct lesions or patches, main color palette including mucosal transitions (such as salmon-pink areas against paler squamous mucosa), visible vascular network patterns, and the presence of any ulceration, exudate, or friability. Do not offer a diagnosis or medical opinion. Just describe the image in a cohesive, natural paragraph.", image_base64_content:str="", model_name=""):
    url = f"http://{llm_api_server_host}:{llm_api_server_port}/api/generate"
    payload = {
        "model": model_name,
        "prompt": prompt_message,
        "images": [image_base64_content],
        "stream": False
    }

    try:
        response = requests.post(url, json=payload)
        response.raise_for_status()
        data = response.json()
        return data.get("response", "[Sem resposta do modelo]")
    except Exception as e:
        return f"Erro ao consultar o modelo: {str(e)}"