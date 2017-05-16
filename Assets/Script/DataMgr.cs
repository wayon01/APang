using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using UnityEngine;

[System.Serializable]
public class DataClass {

    public int CURRENT_STAGE_ID;
    public int CURRENT_STAGE_LOCKED;

    //stage clear data
    public int[] STAGE_DATA;
}

public class DataMgr : MonoBehaviour {

    public DataClass dataClass;

    void Awake() {

        if (!LoadData()) {
            dataClass = new DataClass();

            dataClass.CURRENT_STAGE_ID = 0;
            dataClass.CURRENT_STAGE_LOCKED = 1;

            dataClass.STAGE_DATA = new int[3];
            for (int i = 0; i < 3; i++) {
                dataClass.STAGE_DATA[i] = 0;
            }


            //save
            SaveData();
        }
        
    }

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    public void SaveData() {
        BinaryFormatter _binary_formatter = new BinaryFormatter();
        FileStream _filestream = File.Create(Application.persistentDataPath + "/Data.dat");

        _binary_formatter.Serialize(_filestream, dataClass);

        _filestream.Close();
        
    }

    public bool LoadData() {
        bool _fhile_check = File.Exists(pathForDocumentsFile(Application.persistentDataPath + "/Data.dat"));

        if (!_fhile_check) {
            return false;
        }
        BinaryFormatter _binary_formatter = new BinaryFormatter();

        
        FileStream _filestream = File.Open((Application.persistentDataPath + "/Data.dat"), FileMode.Open);

        
        dataClass = (DataClass)_binary_formatter.Deserialize(_filestream);

        _filestream.Close();

        return true;
    }

    public string pathForDocumentsFile(string filename) {
        if (Application.platform == RuntimePlatform.IPhonePlayer) {
            string path = Application.dataPath.Substring(0, Application.dataPath.Length - 5);
            path = path.Substring(0, path.LastIndexOf('/'));
            return Path.Combine(Path.Combine(path, "Documents"), filename);
        }

        else if (Application.platform == RuntimePlatform.Android) {
            string path = Application.persistentDataPath;
            path = path.Substring(0, path.LastIndexOf('/'));
            return Path.Combine(path, filename);
        }

        else {
            string path = Application.dataPath;
            path = path.Substring(0, path.LastIndexOf('/'));
            return Path.Combine(path, filename);
        }
    }
}
