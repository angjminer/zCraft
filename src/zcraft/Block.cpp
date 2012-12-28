#include "zcraft/Block.hpp"
#include "zcraft/NodeProperties.hpp"
#include "zcraft/face.hpp"
#include "engine/serialization.hpp"

namespace zcraft
{
	Block::~Block()
	{}

	void Block::fill(Node nodeValue)
	{
		dirty = true;
        for(u32 i = 0; i < NODE_COUNT; i++)
            m_nodes[i] = nodeValue;
	}

	void Block::copyTo(VoxelBuffer & vb) const
	{
        Vector3i size(Block::SIZE, Block::SIZE, Block::SIZE);
        Vector3i startPos(
            m_pos.x * Block::SIZE,
            m_pos.y * Block::SIZE,
            m_pos.z * Block::SIZE);

        engine::Area3D area(startPos, size);
        vb.copyFrom(m_nodes, area);
	}

	void Block::copyBorderTo(VoxelBuffer & vb, u8 dir) const
	{
		copyBorderTo(vb, face::toVec3i(dir));
	}

	void Block::copyBorderTo(VoxelBuffer & vb, const Vector3i vdir) const
	{
		// Debug check
		if(!face::isOrthoDirection(vdir))
		{
			std::cout << "ERROR: Block::copyBorderTo: "
				<< "given direction vector is not orthographic "
				<< vdir << std::endl;
			return;
		}

		Vector3i relativeStartPos(0, 0, 0);
		Vector3i relativeEndPos(Block::SIZE - 1, Block::SIZE - 1, Block::SIZE - 1);

		Vector3i startPos(
			m_pos.x * Block::SIZE,
			m_pos.y * Block::SIZE,
			m_pos.z * Block::SIZE);

		if(vdir.x < 0)
			relativeEndPos.x = 0;
		if(vdir.y < 0)
			relativeEndPos.y = 0;
		if(vdir.z < 0)
			relativeEndPos.z = 0;

		if(vdir.x > 0)
			relativeStartPos.x = Block::SIZE - 1;
		if(vdir.y > 0)
			relativeStartPos.y = Block::SIZE - 1;
		if(vdir.z > 0)
			relativeStartPos.z = Block::SIZE - 1;

		Vector3i pos;
		u32 i_src;

		for(pos.z = relativeStartPos.z; pos.z <= relativeEndPos.z; pos.z++)
		for(pos.y = relativeStartPos.y; pos.y <= relativeEndPos.y; pos.y++)
		for(pos.x = relativeStartPos.x; pos.x <= relativeEndPos.x; pos.x++)
		{
			i_src = index(pos.x, pos.y, pos.z);
			vb.set(startPos + pos, m_nodes[i_src]);
		}
	}

	void Block::clear()
	{
        fill(Node());
	}

	bool Block::containsOnlyEmptyNodes() const
	{
        for(u16 i = 0; i < Block::SIZE; i++)
        {
            if(m_nodes[i].type != node::AIR)
                return false;
        }
        return true;
	}

	bool Block::areEdgesFullyOpaque() const
	{
        u16 x, y, z;

        // back and front
        for(x = 0; x < Block::SIZE; x++)
        {
            for(y = 0; y < Block::SIZE; y++)
            {
                z = 0;
                const NodeProperties & p0 = get(x,y,z).properties();
                if(!p0.isOpaqueCube())
                    return false;

                z = Block::SIZE - 1;
                const NodeProperties & p1 = get(x,y,z).properties();
                if(!p1.isOpaqueCube())
                    return false;
            }
        }

        // left and right
        for(z = 0; z < Block::SIZE; z++)
        {
            for(y = 0; y < Block::SIZE; y++)
            {
                x = 0;
                const NodeProperties & p0 = get(x,y,z).properties();
                if(!p0.isOpaqueCube())
                    return false;

                x = Block::SIZE - 1;
                const NodeProperties & p1 = get(x,y,z).properties();
                if(!p1.isOpaqueCube())
                    return false;
            }
        }

        // bottom and top
        for(z = 0; z < Block::SIZE; z++)
        {
            for(x = 0; x < Block::SIZE; x++)
            {
                y = 0;
                const NodeProperties & p0 = get(x,y,z).properties();
                if(!p0.isOpaqueCube())
                    return false;

                y = Block::SIZE - 1;
                const NodeProperties & p1 = get(x,y,z).properties();
                if(!p1.isOpaqueCube())
                    return false;
            }
        }

        return true;
	}

	int Block::getUpperBlockPosition(u8 x, u8 z) const
	{
		s16 y;
        for(y = Block::SIZE-1; y >= 0; y--)
        {
            if(get(x,y,z).type != node::AIR)
                break;
        }
        return y;
	}

	Block * Block::clone() const
	{
        Block * c = new Block(m_pos.x, m_pos.y, m_pos.z);
        memcpy(c->m_nodes, m_nodes, NODE_COUNT * sizeof(Node));
        c->dirty = dirty;
        return c;
	}

	/* Metadata */

	const Vector3i& Block::getPosition() const
	{
		return m_pos;
	}

	std::string Block::toString() const
	{
        std::string res = "Block\n";
        return res;
	}

	/* serialization */

	void Block::serialize(std::ostream & os) const
	{
        /*
            <cx><cy><cz><Node><Node>...<Node>
        */

        // position
        engine::serialize(os, m_pos.x);
        engine::serialize(os, m_pos.y);
        engine::serialize(os, m_pos.z);

        // nodes
        for(u32 i = 0; i < Block::NODE_COUNT; i++)
            m_nodes[i].serialize(os);
	}

	void Block::unserialize(std::istream & is)
	{
        // position
        engine::unserialize(is, m_pos.x);
        engine::unserialize(is, m_pos.y);
        engine::unserialize(is, m_pos.z);

        // nodes
        for(u32 i = 0; i < Block::NODE_COUNT; i++)
            m_nodes[i].unserialize(is);
	}

} // namespace zcraft

