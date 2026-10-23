class AUGSBURG:
    IMAGE_COLUMN = 'img_id'
    PATIENT_ID = 'patient_id'

    LABELS = ['BOD', 'AGE', 'AMD', 'SON', 'AGU', 'BHD', 'BND']
    TARGET_COLUMN = "diagnostic"
    TARGET_NUMBER_COLUMN = "diagnostic_number" #TODOs: tem que mudar o csv pra comportar isso daqui
    SENTENCE = "sentence"