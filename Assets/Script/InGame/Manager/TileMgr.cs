using System;
using System.Collections;
using System.IO;
using System.Text;
using UnityEngine;

public struct TileAreaValue {
    public GameObject front;
    public GameObject back;

    public int id_front;
    public int id_back;

    public TileAreaValue(GameObject objects = null) {
        front = null;
        back = null;
        id_front = id_back = -1;
    }
}

public struct TileArea {
    public TileAreaValue[] x;
    public TileAreaValue[] z;
}

public struct TileShortcut {
    public Vector3 pos;
    public int stage;
    public string name;
    public string targetName;

    public TileShortcut(Vector3 _pos, int _stage, string _name, string _targetName) {
        pos = _pos;
        stage = _stage;
        name = _name;
        targetName = _targetName;
    }
}

public class TileMgr : MonoBehaviour {
    private int _lengthX, _lengthY, _lengthZ;
    private GameObject[,,] _mTileMap;
    private TileArea[] m_tileArea;
    private ArrayList m_tileShortcut;

    private Vector3 _mSelectedTileId;

    public GameObject CameraManager;
    private Camera MainCamera = null;
    private CameraMgr _cameraMgr = null;

    public GameObject GameSystemManager;
    private GameSystemMgr gameSystemMgr;

    public GameObject StageManager;
    private StageMgr stageMgr;

    public GameObject Player;


    //타일 종류
    public GameObject GrassTile = null;
    public GameObject NomalTile = null;
    public GameObject FireTile = null;
    public GameObject SpawnTile = null;
    public GameObject GoalTile = null;
    public GameObject NpcPoliceTile = null;
    public GameObject TreeTile = null;
    public GameObject MushroomTile = null;
    public GameObject FountainTile = null;
    public GameObject PortalTile = null;

    public bool isClicked;
    public bool isLoad;
    private bool isResetTilePosition;

    // Use this for initialization
    void Start() {
        isResetTilePosition = true;
        isClicked = false;
        CameraManager = GameObject.Find("CameraManager");
        _cameraMgr = CameraManager.GetComponent<CameraMgr>();
        gameSystemMgr = GameSystemManager.GetComponent<GameSystemMgr>();
        stageMgr = StageManager.GetComponent<StageMgr>();
        MainCamera = _cameraMgr.MainCamera;
        _mSelectedTileId = new Vector3(-1, -1, -1);
        m_tileShortcut = new ArrayList();
        isLoad = false;

        //TemporaryInit();
        string path = "file://" + Application.streamingAssetsPath + "/test.map";
        //string path = "jar:file://" + Application.streamingAssetsPath + "!/assets/test.map";
        //ParseAndroid(path);

        //Parse(@"./Assets/Maps/test.map");
        StartCoroutine(Parse(path));
        
    }


    // Update is called once per frame
    void Update() {
        if (!isLoad) return;

        ReshapeTilePosition(_cameraMgr.isRelax, (int)_cameraMgr.FinalRotationY);
        if(!gameSystemMgr.isPlayerMoving)
            OnClickTileObject(_cameraMgr.isRelax);

        if (gameSystemMgr.isPlayerMovingUp) {
            AllReshapeTilePosition((int)_cameraMgr.FinalRotationY);
        }

        if (gameSystemMgr.isCleared && gameSystemMgr.m_nextStageId == -1 || gameSystemMgr.isFailed) {
            for (int x = 0; x < _lengthX; x++) {
                for (int y = 0; y < _lengthY; y++) {
                    for (int z = 0; z < _lengthZ; z++) {
                        if (_mTileMap[x,y,z] == null)
                            continue;
                        TileObject tmp = _mTileMap[x, y, z].GetComponent<TileObject>();
                        tmp.SetRigid(true);
                        tmp.SetGravity(true);
                    }
                }
            }

            Rigidbody rigidbody = Player.GetComponent<Rigidbody>();
            rigidbody.useGravity = true;
            rigidbody.isKinematic = false;


        }
    }

    private void ReshapeTilePosition(bool isRelax, int rotationY) {
        if (!isRelax && !isResetTilePosition) {
            isResetTilePosition = true;
            AllResetTilePosition(rotationY);

        }
        else if (isRelax && isResetTilePosition) {
            isResetTilePosition = false;
            AllReshapeTilePosition(rotationY);

        }
    }

    private void AllResetTilePosition(int rotationY) {
        //bool positionIsX = Math.Abs(rotationY) != 90;

        Player.GetComponent<AActor>().resetPosition();
        Player.GetComponent<APlayer>().isBlocked = false;

        //for (int j = 0; j < _lengthY; j++) {

        //    TileAreaValue[] tileVal_X = m_tileArea[j].x;
        //    TileAreaValue[] tileVal_Z = m_tileArea[j].z;

        //    for (int i = 0; i < _lengthX; i++) {

        //        if (tileVal_X[i].id_front != -1)
        //            tileVal_X[i].front.GetComponent<TileObject>().resetPosition();

        //        if (tileVal_X[i].id_back != -1)
        //            tileVal_X[i].back.GetComponent<TileObject>().resetPosition();

        //    }

        //    for (int i = 0; i < _lengthZ; i++) {

        //        if (tileVal_Z[i].id_front != -1)
        //            tileVal_Z[i].front.GetComponent<TileObject>().resetPosition();

        //        if (tileVal_Z[i].id_back != -1)
        //            tileVal_Z[i].back.GetComponent<TileObject>().resetPosition();

        //    }
        //}
    }

    private void AllReshapeTilePosition(int rotationY) {
        bool positionIsX = Math.Abs(rotationY) != 90;
        float deltaPos = !positionIsX ? -_lengthX : -_lengthZ;

        bool isFront = false;

        switch (rotationY) {
            case 0:
            case 90:
                isFront = true; {
                    //Vector3 playerPos = Player.transform.position;
                    //if (!positionIsX) {
                    //    Player.transform.position = new Vector3(-_lengthX, playerPos.y, playerPos.z);
                    //}
                    //else {
                    //    Player.transform.position = new Vector3(playerPos.x, playerPos.y, -_lengthZ);
                    //}
                }

                break;

            case 180:
            case -90:
                isFront = false; {
                    //Vector3 playerPos = Player.transform.position;
                    //if (!positionIsX) {
                    //    Player.transform.position = new Vector3(_lengthX, playerPos.y, playerPos.z);
                    //}
                    //else {
                    //    Player.transform.position = new Vector3(playerPos.x, playerPos.y, _lengthZ);
                    //}
                }

                break;
        }

        for (int j = 0; j < _lengthY; j++) {

            TileAreaValue[] tileVal = positionIsX ? m_tileArea[j].x : m_tileArea[j].z;

            if (isFront)
                setAreaTilePosition(tileVal, deltaPos, positionIsX, true);
            else
                setAreaTilePosition(tileVal, positionIsX ? _lengthZ : _lengthX, positionIsX, false);
        }
    }

    private void setAreaTilePosition(TileAreaValue[] tileVal, float Position, bool positionIsX, bool isFront) {

        AActor player = Player.GetComponent<AActor>();

        for (int i = 0; i < (positionIsX ? _lengthX : _lengthZ); i++){

            if ((isFront ? tileVal[i].id_front : tileVal[i].id_back) == -1)
                continue;

            GameObject obj = isFront ? tileVal[i].front : tileVal[i].back;
            TileObject tileobj = obj.GetComponent<TileObject>();
            float tilePos = Position;

            if (!positionIsX) {
                if (player.positionId.z == tileobj.id.z
                    && player.positionId.y + 1 == tileobj.id.y) {

                    if (isFront ? player.positionId.x > tileobj.id.x
                        : player.positionId.x < tileobj.id.x) {
                        Player.GetComponent<APlayer>().isBlocked = true;
                    }
                    else {
                        tilePos = isFront ? tilePos + 1f : tilePos - 1f;
                        
                    }
                        
                }
                tileobj.SetLocalX(tilePos);
            }
            else {
                if (player.positionId.x == tileobj.id.x
                    && player.positionId.y + 1 == tileobj.id.y) {

                    if (isFront
                        ? player.positionId.z > tileobj.id.z
                        : player.positionId.z < tileobj.id.z) {
                        Player.GetComponent<APlayer>().isBlocked = true;
                    }
                    else {
                        tilePos = isFront ? tilePos + 1f : tilePos - 1f;
                    }
                }
                tileobj.SetLocalZ(tilePos);
            }
        }
    }

    private void OnClickTileObject(bool isRelax) {
        if (Input.GetMouseButtonUp(0) && isRelax) {
            _mSelectedTileId = GetClickedTileId();
            isClicked = true;
        }
    }


    public void SetMapSize(int length_x, int length_y, int length_z) {
        _mTileMap = new GameObject[length_x, length_y, length_z];
        m_tileArea = new TileArea[length_y];

        for (int i = 0; i < length_y; i++) {
            m_tileArea[i].x = new TileAreaValue[length_x];
            m_tileArea[i].z = new TileAreaValue[length_z];

            for (int j = 0; j < length_x; j++)
            {
                m_tileArea[i].x[j] = new TileAreaValue(null);
            }
            for (int j = 0; j < length_z; j++)
            {
                m_tileArea[i].z[j] = new TileAreaValue(null);
            }
        }

        


        _lengthX = length_x;
        _lengthY = length_y;
        _lengthZ = length_z;
    }

    public bool SetTile(GameObject mObject, int x, int y, int z, params string[] additional_val) {
        if (mObject == null) return false;
        if ((x < 0) || (y < 0) || (z < 0)) return false;
        if (_mTileMap[x, y, z] != null) return false;

        var deltaX = _lengthX/2f;
        var deltaY = _lengthY/2f;
        var deltaZ = _lengthZ/2f;

        _mTileMap[x, y, z] = Instantiate(mObject);

        TileObject obj = _mTileMap[x, y, z].GetComponent<TileObject>();

        obj.Position = new Vector3(x - deltaX, y - deltaY, z - deltaZ);
        obj.id = new Vector3(x, y, z);
        obj.resetPosition();

        obj.Init();

        //--

        if (m_tileArea[y].z[z].id_front == -1 ||
                m_tileArea[y].z[z].id_front > x) {
            m_tileArea[y].z[z].front = _mTileMap[x, y, z];
            m_tileArea[y].z[z].id_front = x;
        }

        if (m_tileArea[y].z[z].id_back == -1 ||
                m_tileArea[y].z[z].id_back < x)
        {
            m_tileArea[y].z[z].back = _mTileMap[x, y, z];
            m_tileArea[y].z[z].id_back = x;
        }

        if (m_tileArea[y].x[x].id_front == -1 ||
                m_tileArea[y].x[x].id_front > z)
        {
            m_tileArea[y].x[x].front = _mTileMap[x, y, z];
            m_tileArea[y].x[x].id_front = z;
        }

        if (m_tileArea[y].x[x].id_back == -1 ||
                m_tileArea[y].x[x].id_back < z)
        {
            m_tileArea[y].x[x].back = _mTileMap[x, y, z];
            m_tileArea[y].x[x].id_back = z;
        }
        //===================================
        //가변인자에 대한 내용
        if (additional_val.Length <= 0) return true;

        if (mObject == GoalTile) {
            ((GoalTileObject)obj).SetNextStageId(int.Parse(additional_val[0]));
        }
        else if (mObject == PortalTile) {
            TileShortcut tmp = FindTileShortcut(additional_val[2]);
            if (tmp.name == null) return true;

            ((PortalTile)obj).SetTargetStage(tmp.stage);
            ((PortalTile)obj).SetTargetTileId(tmp.pos);
        }



        return true;
    }


    public GameObject GetTile(int x, int y, int z) {
        return _mTileMap[x, y, z];
    }

    public GameObject GetTile(Vector3 id) {
        return _mTileMap[(int)id.x, (int)id.y, (int)id.z];
    }

    public GameObject GetSurfaceTile(bool isPositionX, bool isSurfaceFront, Vector3 id) {

        TileAreaValue tile = isPositionX ? m_tileArea[(int)id.y].x[(int)id.x]
            : m_tileArea[(int)id.y].z[(int)id.z];

        return isSurfaceFront ? tile.front : tile.back;
    }

    public GameObject GetClickedTile() {
        return _mTileMap[(int)_mSelectedTileId.x, (int)_mSelectedTileId.y, (int)_mSelectedTileId.z];
    }

    public Vector3 GetClickedTileVector3() {
        return _mSelectedTileId;
    }

    public Vector3 GetIdLength() {
        return new Vector3(_lengthX, _lengthY, _lengthZ);
    }

    private Vector3 GetClickedTileId() {
        RaycastHit hit;
        GameObject target = null;
        Ray ray = MainCamera.ScreenPointToRay(Input.mousePosition);
        if (Physics.Raycast(ray.origin, ray.direction * 10, out hit)) {
            target = hit.collider.gameObject;
        }

        if (target == null || !target.CompareTag("Tile")) {
            return new Vector3(-1, -1, -1);
        }

        return target.GetComponent<TileObject>().id;
    }

    private IEnumerator Parse(string mapPath) {
        if (mapPath.Length == 0) yield return null;

        WWW File = new WWW(mapPath);
        yield return File;


        string source;
        string[] sources = File.text.Split('\n');

        if (sources.Length == 0) {
            yield return File;
        }
        

        string[] values = sources[0].Split(',');

        int stageLength = 0;
        StringBuilder stringBuilder = null;
        bool isStageAvailable = false;

        int index = 0;
        //source = sources[index];
        while (index < sources.Length) {
            source = sources[index];
            source = source.Replace(" ", "");
            values = source.Split(',');

            if (source == "#end") {
                break;
            }

            if (values.Length == 0) {
                break;
            }

            //map size
            if (values.Length == 3) {
                if (stringBuilder == null) {
                    stringBuilder = new StringBuilder();
                }
                stringBuilder.AppendLine(source);
                index++;
                //source = fp.ReadLine();
                continue;
            }

            //stage 설정
            if (values[0] == "#STAGE") {
                //첫 스테이지일 경우
                if (!isStageAvailable) {
                    stringBuilder = new StringBuilder();
                    isStageAvailable = true;
                    index++;
                    //source = fp.ReadLine();
                    stageLength = int.Parse(values[1]);
                    continue;
                }
                int tmp = int.Parse(values[1]);
                stageMgr.AddStageString(stringBuilder, stageLength);
                stageLength = tmp;
                stringBuilder = new StringBuilder();
                index++;
                //source = fp.ReadLine();
                continue;

            }

            if (values.Length == 4 || values.Length == 5 && values[0] == "GoalTile") {
                if (stringBuilder == null) {
                    stringBuilder = new StringBuilder();
                }
                stringBuilder.AppendLine(source);
            }

            //포탈타일
            if (values[0] == "PortalTile") {
                TileShortcut tmp = new TileShortcut(new Vector3(int.Parse(values[1]), int.Parse(values[2]), int.Parse(values[3])),
                    stageLength, values[5], values[6]);
                m_tileShortcut.Add(tmp);

                if (stringBuilder == null) {
                    stringBuilder = new StringBuilder();
                }
                stringBuilder.AppendLine(source);
            }
            //ParseTile(values);

            index++;
        }

        if (stringBuilder != null) {
            stageMgr.AddStageString(stringBuilder, stageLength);
        }

        SettingMap(0);
        isLoad = true;
    }

    public void SettingMap(int stageId) {

        if (stageId < 0) return;

        for (int x = 0; x < _lengthX; x++) {
            for (int y = 0; y < _lengthY; y++) {
                for (int z = 0; z < _lengthZ; z++) {
                    Destroy(_mTileMap[x, y, z]);
                }
            }
        }

        StringReader stringReader = new StringReader(stageMgr.GetStageStringReader(stageId).ToString());
        string[] values;
        string source = stringReader.ReadLine();


        while (source != null) {
            source = source.Replace(" ", "");
            values = source.Split(',');
            //map size
            if (values.Length == 3) {
                SetMapSize(int.Parse(values[0]), int.Parse(values[1]), int.Parse(values[2]));
                source = stringReader.ReadLine();
                continue;
            }
            ParseTile(values);
            source = stringReader.ReadLine();

        }

    }

    private IEnumerator ParseAndroid(string mapPath) {
        WWW File = new WWW(mapPath);
        yield return File;

        string source;
        string[] sources = File.text.Split('\n');

        if (sources.Length == 0) {
            yield return File;
        }

        //Map Size
        string[] values = sources[0].Split(',');
        SetMapSize(int.Parse(values[0]), int.Parse(values[1]), int.Parse(values[2]));

        int index = 1;
        source = sources[index];
        while (index >= sources.Length) {
            source = source.Replace(" ", "");
            values = source.Split(',');

            if (values.Length == 0) {
                break;
            }

            ParseTile(values);

            index++;
            source = sources[index];
        }

    }

    private void ParseTile(string[] values) {

        if (values.Length < 4) {
            return;
        }

        GameObject tileObject = null;

        switch (values[0]) {
            case "SpawnTile":
                tileObject = SpawnTile;
                break;
            case "GrassTile":
                tileObject = GrassTile;
                break;
            case "GoalTile": {
                tileObject = GoalTile;
                if (values.Length > 4) {
                    SetTile(tileObject, int.Parse(values[1]), int.Parse(values[2]), int.Parse(values[3]), values[4]);
                    return;
                }
                break;
            }
            case "PortalTile": {
                tileObject = PortalTile;
                SetTile(tileObject, int.Parse(values[1]), int.Parse(values[2]), int.Parse(values[3]), values[4], values[5], values[6]);
                break;
            }
            case "FireTile":
                tileObject = FireTile;
                break;
            case "TreeTile":
                tileObject = TreeTile;
                break;
            case "NpcPoliceTile":
                tileObject = NpcPoliceTile;
                break;
            case "MushroomTile":
                tileObject = MushroomTile;
                break;
            case "FountainTile":
                tileObject = FountainTile;
                break;
            case "NomalTile":
            default:
                tileObject = NomalTile;
                break;
        }

        SetTile(tileObject, int.Parse(values[1]), int.Parse(values[2]), int.Parse(values[3]));
    }

    private TileShortcut FindTileShortcut(string name) {
        for (int i = 0; i < m_tileShortcut.Count; i++) {
            TileShortcut tmp = (TileShortcut)m_tileShortcut[i];

            if (tmp.name == name) {
                return tmp;
            }
        }
        return new TileShortcut(-Vector3.one, -1, null, null);
    }


    //private void TemporaryInit() {
    //    SetMapSize(7, 6, 5);

    //    SetTile(SpawnTile, 0, 0, 0);
    //    SetTile(GrassTile, 1, 0, 1);
    //    SetTile(GrassTile, 2, 0, 0);
    //    SetTile(NomalTile, 3, 1, 4);
    //    SetTile(NomalTile, 0, 0, 2);

    //    SetTile(GrassTile, 1, 4, 1);
    //    SetTile(GrassTile, 1, 4, 2);
    //    SetTile(GrassTile, 2, 5, 4);
    //    SetTile(GoalTile, 3, 5, 4);

    //    SetTile(NomalTile, 4, 2, 3);
        
    //    SetTile(FireTile, 4, 3, 0);
    //    SetTile(NomalTile, 4, 2, 0);
    //    SetTile(NomalTile, 3, 3, 0);


    //    SetTile(FireTile, 6, 0, 0);
    //}
}