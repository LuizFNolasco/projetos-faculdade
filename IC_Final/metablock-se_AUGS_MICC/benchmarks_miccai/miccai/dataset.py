class MICCAI:
    IMAGE_COLUMN = 'img_id'
    PATIENT_ID = 'patient_id'

    LABELS = ['ACHD', 'NDBT']
    TARGET_COLUMN = "diagnostic"
    TARGET_NUMBER_COLUMN = "diagnostic_number" #TODOs: tem que mudar o csv pra comportar isso daqui
    SENTENCE = "sentence"