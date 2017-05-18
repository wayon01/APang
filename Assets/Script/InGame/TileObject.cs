using UnityEngine;

public class TileObject : AGameObject {
    private bool _bIsRigidActivated;
    protected bool _bIsCanIgnoreBlock;
    protected string _speechBubbleStr = "";

    private Rigidbody m_rigidbody;

    public Vector3 id;

    // Use this for initialization
    protected void Start() {
        _bIsRigidActivated = false;
        SetRigidBody(_bIsRigidActivated);
        SetGravity(false);
        _bIsCanIgnoreBlock = false;
    }

    // Update is called once per frame
    protected void Update() {
    }

    // 타일이 생성되고 포지션이 설정된 뒤 초기설정을 하는 함수입니다.
    public virtual void Init() {
        
    }

    public void SetRigid(bool activated) {
        if (_bIsRigidActivated != activated) SetRigidBody(activated);

        _bIsRigidActivated = activated;
    }


    public virtual void SetLocalX(float x) {
        //transform.position = new Vector3(x, transform.position.y, transform.position.z);
    }

    public virtual void SetLocalZ(float z)
    {
        //transform.position = new Vector3(transform.position.x, transform.position.y, z);
    }

    public virtual void OnPlayerMoved() {
        
    }

    public bool IsCanPlayerIgnoreBlock() {
        return _bIsCanIgnoreBlock;
    }

    public void SetGravity(bool activated) {
        if (m_rigidbody == null)
            return;
        m_rigidbody.useGravity = activated;
        m_rigidbody.isKinematic = !activated;
    }

    public string GetSpeechBubble() {
        return _speechBubbleStr;
    }

    public void SetSpeechBubble(string str) {
        _speechBubbleStr = str;
    }

    protected void SetRigidBody(bool activated) {
        if (activated && (m_rigidbody == null))
            m_rigidbody = gameObject.AddComponent<Rigidbody>();

        else if (!activated && (m_rigidbody != null)) {
            Destroy(m_rigidbody);
            m_rigidbody = null;
        }
    }

}