using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DecoTile : TileObject {

    private Collider collider;
    // Use this for initialization
    void Start () {

    }
	
	// Update is called once per frame
	void Update () {
		
	}

    public override void Init() {
        base.Init();
        collider = GetComponent<BoxCollider>();
        _bIsCanIgnoreBlock = true;
        collider.enabled = false;
    }

    public void SetIgnoreBlock(bool activated) {
        _bIsCanIgnoreBlock = !activated;
        //collider.enabled = !activated;
    }
    
}
